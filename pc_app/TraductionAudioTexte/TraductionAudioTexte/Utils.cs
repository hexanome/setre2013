using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Http;
using System.Net.Http.Headers;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Diagnostics;
using System.IO;

namespace TraductionAudioTexte
{
    class Utils
    {
        //public static byte[] ConvertToFlac(byte[] src)
        //{
        //    // Filenames.
        //    string rawFilename = Config.nom_fichier_16 + ".raw";
        //    string flacFilename = Config.nom_fichier_16 + ".flac";

        //    // Write the raw data to a file.
        //    File.WriteAllBytes(rawFilename, src);

        //    // Run process to convert file.
        //    Utils.RunProcess(Config.switch_exe_path, Config.options_switch);

        //    return File.ReadAllBytes(flacFilename);
        //}
        
        public static byte[] Convert8To16Bit(byte[] audio8b7khz)
        {
            var audio16b7khz = new short[audio8b7khz.Length];           // Audio data with 16b resolution at 7312khz.
            var audio16b14khz = new short[audio8b7khz.Length * 2];      // Audio data with 16b resolution at 14624kHz.
            var audio8b8b14khz = new byte[audio16b14khz.Length * 2];    // Audio data with 16b resolution, as an 8b array, at 14625kHz.

            // Convert the signal to 16b.
            int shortRange = short.MaxValue - short.MinValue;
            int multiplier = shortRange / Byte.MaxValue;

            for (int i = 0; i < audio8b7khz.Length; i++)
            {
                audio16b7khz[i] = (short)(short.MinValue + (audio8b7khz[i] * multiplier));
            }

            // Double the frequency by interpolating the missing values.
            for (int i = 0; i < audio16b7khz.Length - 1; i++)
            {
                audio16b14khz[2 * i] = audio16b7khz[i];
                audio16b14khz[2 * i + 1] = (short)((audio16b7khz[i] + audio16b7khz[i + 1]) / 2);
            }

            audio16b14khz[audio16b14khz.Length] = audio16b7khz[audio16b7khz.Length];

            // Write the 16b array as a 8b array.
            for (int i = 0; i < audio16b14khz.Length; i++)
            {
                audio8b8b14khz[2 * i] = (byte)audio16b14khz[i];
                audio8b8b14khz[2 * i + 1] = (byte)(audio16b14khz[i] >> 8);
            }

            return audio8b8b14khz;
        }

        public static byte[] ConvertToFlac(byte[] src)
        {
            using (var proc = new Process()
            {
                EnableRaisingEvents = false,
                StartInfo = new ProcessStartInfo
                {
                    FileName = Config.soxExe,
                    Arguments = string.Format(Config.soxArgs, Config.flacFilename),
                    UseShellExecute = false,
                    RedirectStandardInput = true
                }
            })
            {
                proc.Start();

                // Write the input file.
                proc.StandardInput.BaseStream.Write(src, 0, src.Length);
                proc.StandardInput.Close();

                proc.WaitForExit();

                return File.ReadAllBytes(Config.flacFilename);
            }
        }

        public static async Task<string> TranslateFile(byte[] data)
        {
            var url = "https://www.google.com/speech-api/v1/recognize?xjerr=1&client=chromium&lang=fr-FR";

            // Create the HTTP request.
            var message = new HttpRequestMessage(HttpMethod.Post, url);
            message.Content = new ByteArrayContent(data);
            message.Content.Headers.ContentType = new MediaTypeHeaderValue("audio/x-flac");
            message.Content.Headers.ContentType.Parameters.Add(new NameValueHeaderValue("rate", "16000"));

            // Perform the request.
            var http = new HttpClient();
            var response = await http.SendAsync(message);
            
            // Parse the result as JSON.
            var content = JObject.Parse(await response.Content.ReadAsStringAsync());
            var hypotheses = content["hypotheses"];
            if (hypotheses == null || hypotheses.Count() == 0)
            {
                return null;
            }

            return (string)hypotheses.First["utterance"];
        }
    }
}

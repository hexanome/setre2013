using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Http;
using System.Net.Http.Headers;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace TraductionAudioTexte
{
    class Utils
    {
        
        public static byte[] Convert8To16Bit(byte[] myBytes)
        {
            short[] myShort = new short[myBytes.Length]; // entre 0 et | 257
            short[] myShortDouble = new short[myBytes.Length * 2];
            byte[] myBytes16MHz = new byte[myShortDouble.Length * 2];
            int short_intervalle = short.MaxValue - short.MinValue;
            int multiplieur = short_intervalle / Byte.MaxValue;
            for (int i = 0; i < myBytes.Length; i++)
            {
                short sh = myBytes[i];
                int tampon = short.MinValue + (sh * multiplieur);
                myShort[i] = (short)(tampon);
            }
            for (int i = 0; i < myShort.Length; i++)
            {
                myShortDouble[2 * i] = myShort[i];
                if (i < myShort.Length - 1)
                {
                    short half1 = (short)(myShort[i] / 2);
                    short half2 = (short)(myShort[i + 1] / 2);
                    myShortDouble[2 * i + 1] = (short)(half1 + half2);
                }
                if (i == myShort.Length - 1)
                {
                    myShortDouble[2 * i + 1] = myShort[i];
                }
            }
            for (int i = 0; i < myShortDouble.Length; i++)
            {
                myBytes16MHz[2 * i] = (byte)myShortDouble[i];
                myBytes16MHz[2 * i + 1] = (byte)(myShortDouble[i] >> 8);
            }
            return myBytes16MHz;
        }
        public static void RunProcess(string filename, string args)
        {
            System.Diagnostics.Process proc = new System.Diagnostics.Process();
            proc.EnableRaisingEvents = false;
            proc.StartInfo.FileName = filename;
            proc.StartInfo.Arguments = args;
            proc.Start();
            proc.WaitForExit();
        }
        public static async Task<string> DoStuff(byte[] myBytes)
        {
            var audioFile = myBytes;
            var url = "https://www.google.com/speech-api/v1/recognize?xjerr=1&client=chromium&lang=fr-FR";

            var message = new HttpRequestMessage(HttpMethod.Post, url);
            message.Content = new ByteArrayContent(audioFile);// arrayByte
            message.Content.Headers.ContentType = new MediaTypeHeaderValue("audio/x-flac");
            message.Content.Headers.ContentType.Parameters.Add(new NameValueHeaderValue("rate", "16000"));

            var http = new HttpClient();
            var response = await http.SendAsync(message);

            return await response.Content.ReadAsStringAsync();
        }

    }
}

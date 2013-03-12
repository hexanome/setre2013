using System;
using System.IO;
using System.IO.Ports;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Linq;
using System.Text;
using System.Net.Http;
using System.Net.Http.Headers;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Diagnostics;


namespace TraductionAudioTexte
{
    class Program
    {
        #region Private fields.

        private static SerialPort port;
        private static TaskCompletionSource<bool> receiveComplete;
        private static MemoryStream audioData;
        private static bool readStarted;

        #endregion

        #region Program logic.

        static void Main(string[] args)
        {
            /* Initialize variables. */

            // Port.
            port = new SerialPort(Config.SerialPortParameters.nomPort,
                                  Config.SerialPortParameters.vitesse,
                                  Parity.None,
                                  Config.SerialPortParameters.bitDonnee, StopBits.One);

            // Synchronization mechanism.
            receiveComplete = new TaskCompletionSource<bool>();

            /* Start the actual program. */
            while (true)
            {
                Start().Wait();
            }
        }

        private static async Task Start()
        {            
            using (audioData = new MemoryStream())
            {
                // Open port and start receiving.
                readStarted = false;
                port.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
                port.Open();

                // Wait for the transmission to be over.
                await receiveComplete.Task;

                // Stop receiving data.
                port.DataReceived -= new SerialDataReceivedEventHandler(DataReceivedHandler);

                // Convert the raw data to flac.
                var flacFile = Utils.ConvertToFlac(audioData.GetBuffer());

                // Translate the file.
                string textResult = await Utils.TranslateFile(flacFile);
                if (string.IsNullOrEmpty(textResult))
                {
                    return;
                }
                
                // Write result to serial port.
                port.Write(textResult);
                port.Close();
            }
        }

        #endregion

        #region Event handlers.

        private static void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            var sp = (SerialPort)sender;

            // If the transmission is over, signal it.
            if (sp.BytesToRead == 0 && readStarted)
            {
                receiveComplete.SetResult(true);
                return;
            }

            readStarted = true;

            // Otherwise, copy the buffer to our resulting stream.
            var data = new byte[sp.BytesToRead];
            sp.Read(data, 0, data.Length);
            audioData.Write(data, 0, data.Length);
        }

        #endregion
    }
}

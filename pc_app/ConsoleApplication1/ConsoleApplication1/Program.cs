using System;
using System.IO;
using System.IO.Ports;
using System.Collections.Generic;
using System.Text;
using CUETools.Codecs.FLAKE;

namespace ConsoleApplication1
{
    class Program
    {
       // static DateTime d = new DateTime();
        //static string filename ;= "audio_bin_";// + d.ToLongTimeString();
        private static FileStream fileStream;// = new FileStream(filename, FileMode.Create);
        private static int nbBytes=30;
        static String nomPort = "COM7";
        static int vitesse = 57600;
        static int bitDonnee = 8;

        static void Main(string[] args)
        {
            Console.WriteLine("Initialisation");
            string chaine = ""; 
            
            do
            {
                Console.WriteLine("Menu");
                Console.WriteLine(" - lecture sur le port série taper lecture");
                Console.WriteLine(" - traduction du fichier audio en texte taper traduction");
                Console.WriteLine(" - création d'un fichier vide taper fichier"); // inutile 
                Console.WriteLine(" - transmission texte taper t");
                Console.WriteLine(" - finir taper fin");
                chaine = Console.ReadLine();
                if (chaine == "lecture")
                    LecturePort();
               // if (chaine == "traduction")
                   // Traduction();
                if (chaine == "fichier")
                {
                    DateTime d = DateTime.Now;
                    string datePatt = @"ddMMyyyyhhmmsstt";
                    string filename = "audio_bin_"+d.ToString(datePatt);
                    fileStream = new FileStream(filename, FileMode.Create);
                    fileStream.Close();
                }
                if (chaine == "conversion")
                {
                    Conversion();
                }
                if (chaine == "t")
                {
                    Transmission();
                }
            } while (chaine != "fin");
            Console.WriteLine("Fermeture du programme !!");
           /* do
            {
                chaine = Console.ReadLine();
                Console.Write("Writing ");
                Console.Write(chaine);
                Console.WriteLine(" ...");
                //p.Write(chaine);

            } while (chaine != "end");*/

            
        
        }
        private static void LecturePort()
        {
            
            DateTime d = DateTime.Now;
            string datePatt = @"Mdyyyyhhmmsstt";
            string filename = "audio_bin_" + d.ToString(datePatt);
            fileStream = new FileStream(filename, FileMode.Create);
            SerialPort p = new SerialPort(nomPort, vitesse, Parity.None, bitDonnee, StopBits.One);
            p.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
            p.Open();
            nbBytes = p.BytesToRead; // 0
            Console.WriteLine("Port série ouvert");
            Console.WriteLine("Lecture");
            Console.Read();
            Console.WriteLine("Fermeture de la liaison !!");
            p.Close();
            fileStream.Close();
        }
        private static void Traduction()
        {
            byte[] myBytes = new byte[nbBytes];
            string filename = "audio_bin";
            fileStream = new FileStream(filename, FileMode.Open);
            fileStream.Read(myBytes,0,myBytes.Length);
            fileStream.Close();
            
            
        }
        private static void Conversion()
        {
            string filename = "audio_bin2";
            byte[] myBytes =File.ReadAllBytes(filename); // entre 0 et 255 short
            short[] myShort = new short[myBytes.Length]; // entre 0 et | 257
            short[] myShortDouble = new short[myBytes.Length*2];
            byte[] myBytes16MHz = new byte[myShortDouble.Length * 2];

            int short_intervalle = short.MaxValue - short.MinValue;
            int multiplieur = short_intervalle/Byte.MaxValue;
            for (int i = 0; i < myBytes.Length; i++)
            {
                short sh = myBytes[i];
                int tampon = short.MinValue + (sh * multiplieur);
                myShort[i] = (short)(tampon);
            }
            for (int i = 0; i < myShort.Length; i++)
            {
                myShortDouble[2*i] = myShort[i];
                if (i < myShort.Length-1)
                {
                    short half1 = (short)(myShort[i] / 2);
                    short half2 = (short)(myShort[i + 1] / 2);
                    myShortDouble[2 * i + 1] = (short)(half1 + half2);
                }
                if (i == myShort.Length - 1)
                {
                    myShortDouble[2 * i+1] = myShort[i];
                }
            }
            for (int i = 0; i < myShortDouble.Length; i++)
            {
                myBytes16MHz[2*i] = (byte)myShortDouble[i];
                myBytes16MHz[2*i+1] = (byte)(myShortDouble[i] >> 8);
            }
           /* DateTime d = DateTime.Now;
            string datePatt = @"Mdyyyyhhmmsstt";
            string filename2 = "audio_raw_" + d.ToString(datePatt);
            FileStream fileStream2 = new FileStream(filename2, FileMode.Create);
            fileStream2.Write((byte[])myBytes16MHz, 0, myBytes16MHz.Length);
            fileStream2.Close();*///MemoryStream get buffer
            DateTime d = DateTime.Now;
            string datePatt = @"Mdyyyyhhmmsstt";
            Console.WriteLine("Conversion finie !!");
            string outputFile = "audio_flac_" + d.ToString(datePatt) + ".flac";
            FlakeWriter flac = new FlakeWriter(null, 16, 1, 16000, File.Create(outputFile));
            //FlacWriter flac = new FlacWriter(File.Create(outputFile), 16, 1, 16000);
            //flac.Write(myBytes16MHz, 0, myBytes16MHz.Length);
            //flac.Write();
            flac.Close();
           
        }
        private static void Transmission()
        {
            SerialPort p = new SerialPort(nomPort, vitesse, Parity.None, bitDonnee, StopBits.One);
            p.Open();
            string message = "bonjour a afficher sur l'écran LCD";
           /* for (int i = 0; i <= message.Length; i++)
            {
                string m= new char[1];
                m[0] = message[i];
                p.Write(m, 0, 1);// ecrire caractere par caractere
            }*/
            Console.WriteLine(" ");
            Console.WriteLine("Transmission");
            Console.WriteLine(" ");
            p.Write(message);
            p.Close();
        }
       /* private static async Task<string> DoStuff(var audioFile)
        {
            //var audioFile = File.ReadAllBytes(filename);
            var url = "https://www.google.com/speech-api/v1/recognize?xjerr=1&client=chromium&lang=fr-FR";

            var message = new HttpRequestMessage(HttpMethod.Post, url);
            message.Content = new ByteArrayContent(audioFile);// arrayByte
            message.Content.Headers.ContentType = new MediaTypeHeaderValue("audio/x-flac");
            message.Content.Headers.ContentType.Parameters.Add(new NameValueHeaderValue("rate", "16000"));

            var http = new HttpClient();
            var response = await http.SendAsync(message);

            return await response.Content.ReadAsStringAsync();
        }*/
        private static void DataReceivedHandler(
                      object sender,
                      SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            byte[] myBytes = new byte[sp.BytesToRead];
            sp.Read(myBytes, 0, myBytes.Length);
            Console.WriteLine("ecriture");
            fileStream.Write(myBytes,0,myBytes.Length);
        }
    }
}

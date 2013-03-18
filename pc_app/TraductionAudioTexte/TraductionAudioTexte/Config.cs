using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TraductionAudioTexte
{
    class Config
    {
        public struct SerialPortParameters
        {
            public static int NbBytes = 30;
            public static String NomPort = "COM12";
            public static int Vitesse = 230400;
            public static int BitDonnee = 8;
        }

        public static string flacFilename = "audio.flac";
        public static string soxExe = "External\\sox.exe";
        public static string soxArgs = "-t raw -e unsigned-integer -r 8192 -b 8 -c 1 − -b 16 -t flac {0} rate -v 16k";
    }
}

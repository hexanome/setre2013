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
            public static int nbBytes = 30;
            public static String nomPort = "COM7";
            public static int vitesse = 57600;
            public static int bitDonnee = 8;
        }

        public static string flacFilename = "audio.flac";
        public static string soxExe = "External\\sox.exe";
        public static string soxArgs = "-t raw -e unsigned-integer -r 7312 -b 8 -c 1 − -b 16 -t flac {0} rate -v 16k";
    }
}

using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace HelloButton
{
    public class StropBench
    {

        private DateTime mTimer1;
        private const double RUNNING_TIME = 2;
        private Random gRand = new Random();

        public StropBench() //Constructor
        {
          
        }

        public string bench()
        {
            double time;
            int i;
            string ret = "";

            for (i = 1; (time = appender(i)) < RUNNING_TIME; i *= 2) ; //run until time >= RUNNING_TIME
            ret += string.Format("append ran for {0:0.0}s: {1:0.00} KSTROPS\r\n", time, i / time*0.001);

            for (i = 1; (time = substring(i)) < RUNNING_TIME; i *= 2) ; //run until time >= RUNNING_TIME
            ret += string.Format("substring ran for {0:0.0}s: {1:0.00} KSTROPS\r\n", time, i / time * 0.001);

            for (i = 1; (time = remove(i)) < RUNNING_TIME; i *= 2) ; //run until time >= RUNNING_TIME
            ret += string.Format("remove ran for {0:0.0}s: {1:0.00} KSTROPS\r\n", time, i / time * 0.001);

            for (i = 1; (time = contains(i)) < RUNNING_TIME; i *= 2) ; //run until time >= RUNNING_TIME
            ret += string.Format("contains ran for {0:0.0}s: {1:0.00} KSTROPS\r\n", time, i / time * 0.001);

            for (i = 1; (time = insert(i, 0)) < RUNNING_TIME; i *= 2) ; //run until time >= RUNNING_TIME
            ret += string.Format("insert(stringvar) ran for {0:0.0}s: {1:0.00} KSTROPS\r\n", time, i / time * 0.001);

            for (i = 1; (time = insert(i, 1)) < RUNNING_TIME; i *= 2) ; //run until time >= RUNNING_TIME
            ret += string.Format("insert(stringconst) ran for {0:0.0}s: {1:0.00} KSTROPS\r\n", time, i / time * 0.001);

            return ret;
        }

        double appender(int numRuns)
        {
            startTime();
            string appStr = "append me!";
            for (int i = 0; i < numRuns; ++i)
            {
                appStr += "junk";
            }
            return elapsedTime();
        }

        double substring(int numRuns)
        {
            int pos;
            startTime();
            string longStr = "abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz";
            string subStr = "";
            for (int i = 0; i < numRuns; ++i)
            {
                subStr = longStr.Substring(pos = gRand.Next() % (longStr.Length - 1), gRand.Next() % (longStr.Length - pos));
            }
            return elapsedTime();
        }

        double remove(int numRuns)
        {
            startTime();
            string longStr = "";
            int pos;
            for (int i = 0; i < numRuns; ++i)
            {
                longStr = "abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz";
				longStr.Remove(pos = gRand.Next()%(longStr.Length-1), gRand.Next()%(longStr.Length-pos)); //randomize starting position from 0 -> strlen-1 and length from 0->strlen-startpos
            }
            return elapsedTime();
        }

        double contains(int numRuns)
        {
            startTime();
            string longStr = "abcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyzabcdefghiljkmnopqrstuvwxyz";
            string alphabet = "abcdefghiljkmnopqrstuvwxyz";
            for (int i = 0; i < numRuns; ++i)
            { 
                longStr.Contains(alphabet);
            }
            return elapsedTime();
        }

        double insert(int numRuns, int insType)
        {
            startTime();
            string str = "I am a string!";
	        string inStr = "I am a string to be inserted into other strings!";
            for (int i = 0; i < numRuns; ++i)
            {
                if (insType == 0)
                    str.Insert(4, inStr);
                else
                    str.Insert(4, "I am a string to be inserted into other strings!");
            }
            return elapsedTime();
        }

        void startTime() //set the timer to the current time
        {
            mTimer1 = DateTime.Now;
        }

        double elapsedTime() //return the number of seconds since last call to startTime()
        {
            TimeSpan ts = DateTime.Now - mTimer1;
            return ts.TotalSeconds;
        }

    }
}

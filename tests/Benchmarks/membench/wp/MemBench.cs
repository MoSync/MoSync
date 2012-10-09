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
using System.Collections.Generic;

namespace MemBench
{
    public class MemBench
    {

        private DateTime mTimer1;
        private const double RUNNING_TIME = 2;
        List<int> mIntVec = new List<int>();

        /* TEST TYPES */
        /* MALLOCS */
        const int STRING = 0;
        const int DUMMY_OBJ = 1;
        const int DUMMY_STRUCT = 2;
        const int DUMMY_MIX = 3;

        /* MEM ACCESS */
        const int ARRAY = 0;
        const int VECTOR_ADD = 1;
        const int VECTOR_ACCESS = 2;
        const int DUMMY_ACCESS = 3;
        const int DUMMY_MIX_ACCESS = 4;
        const int DUMMY_STRUCT_ACCESS = 5;

        public MemBench()
        {

        }

        private class DummyClass
        {
            public int a, b, c;
            public DummyClass()
            {
              
            }
        }

        private class DummyObjectMix
        {
            int getA(){return a;}
	        int getB(){return b;}
	        int getC(){return c;}
	        float getD(){return d;}
	        float getE(){return e;}
	        float getF(){return f;}
	        float getG(){return g;}

	        List<string> getStrVec(){return str_vec;}
	        string getStr(){return str;}

            int a;
            int b;
            int c;
            float d;
            float e;
            float f;
            float g;
            List<string> str_vec;
            string str;
        }

        struct DummyStruct
        {
            int a, b, c;
        }

        public string bench()
        {
            double time;
            int i;
            string ret = "";

            for (i = 1; (time = heapBench(i, 10, STRING)) < RUNNING_TIME; i *= 2) ; //run until time >= RUNNING_TIME
            ret += string.Format("heapBench(string(10)) ran for {0:0.0}s: {1:0.00} KMEMOPS\r\n", time, i / time * 0.001);

            for (i = 1; (time = heapBench(i, 100, STRING)) < RUNNING_TIME; i *= 2) ; //run until time >= RUNNING_TIME
            ret += string.Format("heapBench(string(100)) ran for {0:0.0}s: {1:0.00} KMEMOPS\r\n", time, i / time * 0.001);

            for (i = 1; (time = heapBench(i, -1, DUMMY_OBJ)) < RUNNING_TIME; i *= 2) ; //run until time >= RUNNING_TIME
            ret += string.Format("heapBench(dummy_obj) ran for {0:0.0}s: {1:0.00} KMEMOPS\r\n", time, i / time * 0.001);

            for (i = 1; (time = heapBench(i, -1, DUMMY_STRUCT)) < RUNNING_TIME; i *= 2) ; //run until time >= RUNNING_TIME
            ret += string.Format("heapBench(dummy_struct) ran for {0:0.0}s: {1:0.00} KMEMOPS\r\n", time, i / time * 0.001);

            for (i = 1; (time = heapBench(i, -1, DUMMY_MIX)) < RUNNING_TIME; i *= 2) ; //run until time >= RUNNING_TIME
            ret += string.Format("heapBench(dummy_mix) ran for {0:0.0}s: {1:0.00} KMEMOPS\r\n", time, i / time * 0.001);

            for (i = 1; (time = memAccess(i, 100, ARRAY)) < RUNNING_TIME; i *= 2) ; //run until time >= RUNNING_TIME
            ret += string.Format("memaccess(array(100)) ran for {0:0.0}s: {1:0.00} KMEMOPS\r\n", time, i*100 / time * 0.001); //i*100 because we want to measure each memaccess

            for (i = 1; (time = memAccess(i, 100, VECTOR_ADD)) < RUNNING_TIME; i *= 2) ; //run until time >= RUNNING_TIME
            ret += string.Format("memaccess(vector_add(100)) ran for {0:0.0}s: {1:0.00} KMEMOPS\r\n", time, i * 100 / time * 0.001); //i*100 because we want to measure each memaccess

            for (i = 1; (time = memAccess(i, 100, VECTOR_ACCESS)) < RUNNING_TIME; i *= 2) ; //run until time >= RUNNING_TIME
            ret += string.Format("memaccess(vector(100)) ran for {0:0.0}s: {1:0.00} KMEMOPS\r\n", time, i * 100 / time * 0.001); //i*100 because we want to measure each memaccess

            for (i = 1; (time = memAccess(i, 100, DUMMY_ACCESS)) < RUNNING_TIME; i *= 2) ; //run until time >= RUNNING_TIME
            ret += string.Format("memaccess(dummy) ran for {0:0.0}s: {1:0.00} KMEMOPS\r\n", time, i * 100 / time * 0.001); //i*100 because we want to measure each memaccess

            return ret;
        }

        /* allocate strings to the heap */
        private double heapBench(int numRuns, int size, int testType)
        {
            startTime();

            switch(testType){
                case STRING:
                    for (int i = 0; i < numRuns; ++i)
                    {
                        String str = new String('\0', size);
                    }
                break;

                case DUMMY_OBJ:
                    for (int i = 0; i < numRuns; ++i)
                    {
                        new DummyClass();
                    }
                break;

                case DUMMY_STRUCT:
                    for (int i = 0; i < numRuns; ++i)
                    {
                        new DummyStruct();
                    }
                break;

                case DUMMY_MIX:
                    for (int i = 0; i < numRuns; ++i)
                    {
                        new DummyObjectMix();
                    }
                break;
            }   
            return elapsedTime();
        }

        double memAccess(int numRuns, int size, int testType)
        {
            char[] charArray = new char[size];
            DummyClass dummyO = new DummyClass(); //dummy_object
            startTime();

            switch (testType)
            {
                case ARRAY:
                    for (int i = 0; i < numRuns; ++i)
                    {
                        for (int k = 0; k < size - 1; ++k)
                        {
                            charArray[k] = charArray[k + 1];
                        }
                    }
                    break;

                case VECTOR_ADD:
                    for(int i = 0; i < numRuns; ++i)
                    {
				        mIntVec.Clear();
				        for(int k = 0; k < size; ++k)
                        {
					        mIntVec.Add(k);
				        }
		
		            }
                    break;

                case VECTOR_ACCESS:
                    for (int i = 0; i < numRuns; ++i)
                    {
                        for (int k = 0; k < size-1; ++k) //-1 otherwise oob
                        {
                            mIntVec[k] = mIntVec[k + 1];
                        }
                    }
                    break;

                case DUMMY_ACCESS:
                    for (int i = 0; i < numRuns; ++i)
                    {
                        for (int k = 0; k < size; ++k)
                        {
                            dummyO.a = k;
                            dummyO.b = dummyO.c;
                            dummyO.c = i;
                        }
                    }
                    break;

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

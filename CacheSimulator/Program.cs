using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CacheSimulator
{
    class Program
    {
        //variables to keep the configuration data
        private static int noOfSets;        
        private static int sizeOfLine;
        private static int associative;
        static void Main(string[] args)
        {
            try
            {
                // Create an instance of StreamReader to read from a file.
                // The using statement also closes the StreamReader.
                using (StreamReader sr = new StreamReader("trace.config"))
                {

                    //Splits one line ata time and distributes its values to the variables as needed.
                    string[] words = sr.ReadLine().Split(':');
                    noOfSets = Convert.ToInt32(words[1].Trim());

                    words = sr.ReadLine().Split(':');
                    associative = Convert.ToInt32(words[1].Trim());

                    words = sr.ReadLine().Split(':');
                    sizeOfLine = Convert.ToInt32(words[1].Trim());

                    Console.WriteLine("Cache Configuration");
                    Console.WriteLine("\t" + noOfSets + " " + associative + "- way set associative entries");
                    Console.WriteLine("\tof line size " + sizeOfLine + " bytes");

                }

                string[] lines = File.ReadAllLines("trace.dat");

                //Create a Cache object with the specified configuration
                Cache c = new Cache(noOfSets,associative,sizeOfLine);
                //Create a CacheStatus object to keep track of each Write and Read operations
                CacheStatus cs= new CacheStatus();

                Console.WriteLine("Result for Each Reference");
                Console.WriteLine("Ref\tAccess\tAddress\tTag\tIndex\tOffset\tResult\tMemrefs");

                int count = 1;
                float hit = 0f;
                float miss = 0f;
                foreach (string line in lines)
                {
                    try
                    {
                        //Spliting single line at a time to retrieve the operation type('R' or 'W') 
                        //reference Size and address in hexadecimal
                        string[] words = line.Split(':');
                        String access = words[0];
                        int refSize = Convert.ToInt32(words[1]);
                        
                        //Check if it is a Read or Write statement
                        if (access.Equals("R"))
                        {
                            //Read from Cache sending the hex value
                            cs = c.readCache(words[2]);
                            
                        }
                        else
                        {
                            //Write to Cache sending the hex value
                            cs = c.writeCache(words[2]);
                        }

                        //Counting Miss and Hit
                        if (cs.result.Equals("Miss"))
                        {
                            ++miss;
                        }
                        else
                        {
                            ++hit;
                        }

                        //Print out the result
                        Console.WriteLine(count + "\t" + cs.access + "\t" + cs.address + "\t" + cs.tag + "\t" + cs.index + "\t" + cs.offset + "\t" + cs.result + "\t" + cs.memRefs.ToString());
                        count++;
                    }catch(Exception e){
                        Console.WriteLine("Error : "+e.Message);
                    }
                }

                Console.WriteLine("Simulation Summary Statistics");
                Console.WriteLine("Total hits\t: "+hit);
                Console.WriteLine("Total misses\t: "+miss);
                Console.WriteLine("Total accesses\t: "+(hit+miss));
                Console.WriteLine("Hit ratio\t: "+(hit / (hit + miss)));
                Console.WriteLine("Miss ratio\t: "+(miss / (hit + miss)));

                //Printing the Cache
                c.displayCache();

            }
            catch (Exception e)
            {
                Console.WriteLine("Error : " + e.Message);                                
            }


            //Wait for any key to press
            //Will be helpful if running using Visual Studio IDE
            Console.ReadKey();
        }
    }
}

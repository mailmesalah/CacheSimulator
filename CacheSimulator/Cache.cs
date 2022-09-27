using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CacheSimulator
{

    class Cache
    {
        int[,] tag;
        int[,] index;
        int[,] offset;
        int[,] valid;//0 is invalid all other numbers valid bigger number most recently accessed
        int[,] writeAllocate;
        int associative;
        int noOfSets;
        int lineSize;
        public Cache(int noOfSets,int associative,int lineSize){
            //Initialises the variables
            tag= new int [associative,noOfSets/associative];
            index = new int[associative, noOfSets / associative];
            offset = new int[associative, noOfSets / associative];
            valid = new int[associative, noOfSets / associative];
            writeAllocate = new int[associative, noOfSets / associative];
            
            this.associative = associative;
            this.noOfSets = noOfSets;
            this.lineSize = lineSize;
            
        }

        public CacheStatus readCache(string sAddress){            
            CacheStatus cs = new CacheStatus();
            //hexadecimal value in string is converted to interger value
            int address = Convert.ToInt32(sAddress, 16);
            //Ofset index is obtained
            int offsetIndex = address % lineSize;
            int partAdd = address / lineSize;
            //tag index is obtained
            int tagIndex = partAdd / (noOfSets / associative);
            //index value is obtained
            int indexValue = partAdd % (noOfSets / associative);

            //Console.WriteLine(indexValue+" tag->"+tagIndex+" add->"+address);
            Boolean hit = false;
            
            //Check if there is a hit
            for (int i = 0; i <=associative-1 ; i++)
			{
                //checking if valid and tag index already in cache
                if (valid[i, indexValue] != 0 && tag[i, indexValue] == tagIndex)
                {
                    hit = true;
                    valid[i, indexValue]++;//increment to set it used recently
                    tag[i, indexValue] = tagIndex;

                    //Return Cache Status
                    cs.access = "Read";
                    cs.address = sAddress;
                    cs.tag=tagIndex;
                    cs.index=indexValue;
                    cs.offset = offsetIndex;
                    cs.result="Hit";
                    cs.memRefs = 0;

                    break;
                } 
			}

            //If not hit register for first time in proper place
            if (!hit)
            {
                Boolean found = false;
                //Check if free space available
                for (int i = 0; i <= associative - 1; i++)
                {
                    if (valid[i, indexValue] == 0)
                    {
                        found = true;
                        valid[i, indexValue]++;//increment to set it used recently
                        tag[i, indexValue] = tagIndex;
                        
                        //Return Cache Status
                        cs.access = "Read";
                        cs.address = sAddress;
                        cs.tag = tagIndex;
                        cs.index = indexValue;
                        cs.offset = offsetIndex;
                        cs.result = "Miss";
                        cs.memRefs = 1;
                        
                        break;
                    }
                }

                //if no freespace available, over write the least used cache block
                if (!found)
                {
                    //Find the least used set
                    int lowest = valid[0, indexValue];
                    int ind=0;
                    for (int i = 1; i <= associative - 1; i++)
                    {
                        if (lowest < valid[i, indexValue])
                        {
                            lowest = valid[i, indexValue];
                            ind = i;
                        }                        
                    }

                    //LRU replacement
                    valid[ind, indexValue]++;//increment to set it used recently
                    tag[ind, indexValue] = tagIndex;

                    //Return Cache Status
                    cs.access = "Read";
                    cs.address = sAddress;
                    cs.tag = tagIndex;
                    cs.index = indexValue;
                    cs.offset = offsetIndex;
                    cs.result = "Miss";
                    cs.memRefs = 1;
                }
            }
            
            return cs;
        }

        public CacheStatus writeCache(string sAddress)
        {
            CacheStatus cs = new CacheStatus();
            //Converting hexadecimal value in sting to Integer
            int address = Convert.ToInt32(sAddress, 16);
            //offset index is obtained
            int offsetIndex = address % lineSize;
            int partAdd = address / lineSize;
            //Tag Index is obtained
            int tagIndex = partAdd / (noOfSets / associative);
            //Index value is obtained from address
            int indexValue = partAdd % (noOfSets / associative);

            //For implementing Write Allocate and Write Back policies
            writeAllocate = new int[associative, noOfSets / associative];

            //Console.WriteLine(indexValue+" tag->"+tagIndex+" add->"+address);
            Boolean hit = false;

            //Check if there is a hit
            for (int i = 0; i <= associative - 1; i++)
            {
                //Check if valid and tag Index is already in Cache
                if (valid[i, indexValue] != 0 && tag[i, indexValue] == tagIndex)
                {
                    hit = true;
                    valid[i, indexValue]++;//increment to set it used recently
                    tag[i, indexValue] = tagIndex;
                    
                    //Write Allocate
                    writeAllocate[i, indexValue] = 1;

                    //Return Cache Status
                    cs.access = "Write";
                    cs.address = sAddress;
                    cs.tag = tagIndex;
                    cs.index = indexValue;
                    cs.offset = offsetIndex;
                    cs.result = "Hit";
                    cs.memRefs = 0;

                    
                    break;
                }
            }

            //If not hit register for first time in proper place
            if (!hit)
            {
                Boolean found = false;
                //Check if free space available
                for (int i = 0; i <= associative - 1; i++)
                {
                    if (valid[i, indexValue] == 0)
                    {
                        found = true;
                        valid[i, indexValue]++;//increment to set it used recently
                        tag[i, indexValue] = tagIndex;

                        //Write Allocate
                        writeAllocate[i, indexValue] = 1;

                        //Return Cache Status
                        cs.access = "Write";
                        cs.address = sAddress;
                        cs.tag = tagIndex;
                        cs.index = indexValue;
                        cs.offset = offsetIndex;
                        cs.result = "Miss";
                        cs.memRefs = 0;

                        break;
                    }
                }

                //If no free space available, over write the least used cache block
                if (!found)
                {
                    //Find the least used set
                    int lowest = valid[0, indexValue];
                    int ind = 0;
                    for (int i = 1; i <= associative - 1; i++)
                    {
                        if (lowest < valid[i, indexValue])
                        {
                            lowest = valid[i, indexValue];
                            ind = i;
                        }
                    }

                    //LRU replacement
                    valid[ind, indexValue]++;//increment to set it used recently
                    tag[ind, indexValue] = tagIndex;

                    //Return Cache Status
                    cs.access = "Write";
                    cs.address = sAddress;
                    cs.tag = tagIndex;
                    cs.index = indexValue;
                    cs.offset = offsetIndex;
                    cs.result = "Miss";
                    cs.memRefs = writeAllocate[ind, indexValue]==1?1:0;
                    
                    //Write Allocate
                    writeAllocate[ind, indexValue] = 1;
                }
            }

            return cs;
        }

        public void displayCache()
        {
            //Displaying the Cache current state
            Console.WriteLine("Inside Cache");
            for (int j = 0; j < noOfSets/associative; j++)
            {
                string row = "";
                for (int i = 0; i < associative; i++)
                {
                    row += "V:"+valid[i, j]+" - Tag:"+tag[i,j]+" - Index:"+j+"\t";
                }
                Console.WriteLine(row);
                
            }
        }
    }
}

//CSE 141 - Su07
//Cache Simulator starter code (revision 3, 09-02-2007)
//Cynthia Lee

import java.io.*;
import java.util.*;

//
//Execute this program with the following arguments:
//   
// - name of the memory trace input file
//
//The following are optional arguments:
//
// -b <block size (in bytes) - default 4>
// -l <number of cache lines - default 8>
// -a <associativity - default 1 = Direct Mapped>
//

// THE STARTER CODE ONLY IMPLEMENTS THE DEFAULT VALUES

public class CacheSim {

	// Data storage
	private int [] tags;
	private int blocksize = 4;
	private int nlines = 8;
	private int assoc = 1;
	private boolean randReplacement = false;
	private FileReader inputFile = null;

	public void parseArgs(String[] args) {
		int index = 0;
		boolean gotInput = false;

		while(index < args.length) {
			if(args[index].equalsIgnoreCase("-a")) {
				assoc = Integer.parseInt(args[index+1]);
				index = index + 2;
			}
			else if(args[index].equalsIgnoreCase("-b")) {
				blocksize = Integer.parseInt(args[index+1]);
				index = index + 2;
			}
			else if(args[index].equalsIgnoreCase("-l")) {
				nlines = Integer.parseInt(args[index+1]);
				index = index + 2;
			}
			else if(args[index].equalsIgnoreCase("-rand")) {
				randReplacement = true;
				index++;
			}
			else if(args[index].endsWith(".input")) {
				try {
					inputFile = new FileReader(args[index]);
					gotInput = true;
					index++;
	   			}
	   			catch (Exception e) {
					e.printStackTrace();
				}
			}
			else {
				System.err.println("unknown argument: " + args[index]);
				System.exit(1);
			}

		}

		if(!gotInput) {
			System.err.println("need to specify an input file!");
			System.exit(1);
		}
	}

	public void run() 
	{
	   try{

		//This needs to change to accomodate 
		tags = new int[nlines];
		System.out.println("Cache Design:");
		System.out.println("  Block Size = \t\t" + blocksize + " bytes");
		System.out.println("  Cache Lines = \t" + nlines);

		if(assoc == 1)
			System.out.println("  Associativity = \tDirect Mapped");
		else if(assoc == nlines)
			System.out.println("  Associativity = \tFully Associative");
		else	
			System.out.println("  Associativity = \t" + assoc + "-way");

		System.out.println("  Size = \t\t" + "XXXX" + " bytes");
		System.out.println("  Data Size = \t\t" + "XXXX" + " bytes");
		System.out.println("  Overhead = \t\t" + "XXXX");

		BufferedReader trace = new BufferedReader(inputFile);
		String line = null;

		int nHits = 0;
		int nAccess = 0;

		/* begin new rev 3 stuff */
		int nLoadAccess = 0; // number of loads
		int nStoreAccess = 0; // number of stores
		int nLoadHits = 0; // how many of the loads were hits
		int nStoreHits = 0; // how many of the store accesses were hits
		int nEvicted = 0; // number of dirty blocks that were evicted from cache
		double totalHitRate;  // overall hit rate
		double loadHitRate; // hit rate for loads
		double storeHitRate;  // hit rate for stores
		/* end rev 3 stuff */

		while ((line = trace.readLine()) != null) {
			StringTokenizer st = new StringTokenizer(line);
			String type = st.nextToken();//load or store
			int addr = Integer.parseInt(st.nextToken());

			int offset = (addr << 30) >>> 30;
			int index = ((addr >>> 2) << 29) >>> 29;
			int tag = addr >>> 5;

			tags[index] = tag;

		}


		/* begin new rev 3 stuff */

		// calculate hit rates
		totalHitRate = (double) nHits / (double) nAccess;
		loadHitRate = (double) nLoadHits / (double) nLoadAccess;
		storeHitRate = (double) nStoreHits / (double) nStoreAccess;

		// print cache stats
		System.out.println("Cache Statistics:");
		System.out.println("  Total accesses: " + nAccess + " (" + nLoadAccess + " loads, " + nStoreAccess + " stores)");
		System.out.println("  Hit rate: \n    Total: " + totalHitRate + " (" + nHits + " out of " + nAccess + ")\n    Loads: "+ loadHitRate + " (" + nLoadHits + " out of " + nLoadAccess + ")\n    Stores: " + storeHitRate + " stores (" + nStoreHits + " out of " + nStoreAccess + ")");
		System.out.println("  Dirty blocks evicted (AKA written back): " + nEvicted);

		/* end rev 3 stuff */

		trace.close();
	   }
	   catch (Exception e) {
		e.printStackTrace();
	   }
	}

	//This should prove useful to you in doing offset/index/tag
	//calculations with variable parameters for # cache lines, etc.
	private int log2 (int x) {
		return (int)(Math.log(x)/Math.log(2));
	}


	public static void main(String [] args) {

		CacheSim c = new CacheSim();
		c.parseArgs(args);
		c.run();
	}
}

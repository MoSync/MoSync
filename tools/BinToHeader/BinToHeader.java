/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

import java.io.*;

public class BinToHeader {

	public static void main(String args[]) throws IOException {
		if (args.length < 2) {
			System.out.println("Usage: java BinToHeader <infile> <outfile>");// <piece_length>");
			return;
		}

		DataInputStream dis = new DataInputStream(new FileInputStream(args[0]));
		DataOutputStream dos = new DataOutputStream(new FileOutputStream(args[1] + ".h"));

		boolean finished = false;
		boolean first = true;

		byte[] input = new byte[dis.available()];
		System.out.println("Size: " + input.length);
		dis.read(input);

		dos.writeBytes("static final byte[] " + args[1] + " = {");
		for (int i = 0; i < input.length; i++) {
			dos.writeBytes((i == 0 ? "" : ",") + ((int)input[i]));
		}
		dos.writeBytes("};\n");

		/*
		int n = Integer.parseInt(args[2]);

		byte b = 0;
		int i = 0;
		int j = 0;
		while (true) {
			dos.writeBytes("static final byte[] " + args[1] + i + " = {");
			j = 0;
			while (true) {

				if (first == false) {
					j++;
					if (j % n == 0) {
						break;
					} else {
						dos.writeBytes((((int)b)) + ", ");
					}
				}
				first = false;

				try {
					b = dis.readByte();
				} catch (EOFException e) {
					finished = true;
					break;
				}
			}
			dos.writeBytes("};\n");
			if (finished == true)
				break;
			i++;
		}*/

		dis.close();
		dos.close();
	}

}

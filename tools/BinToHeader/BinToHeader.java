/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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

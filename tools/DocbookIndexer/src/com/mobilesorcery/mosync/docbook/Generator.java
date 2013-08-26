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

/**
 * MoSync Documentation Parser 
 * XML + dokuwiki syntax based documentation
 * 
 * Copyright © Mobile Sorcery 
 * 
 * Author: Aaron Chan
 * 
 * Generator.java - Base Module for generating output
 * 
 * Last edited: 20 May 2008
 * 
 */

package com.mobilesorcery.mosync.docbook;

import java.awt.image.BufferedImage;
import java.io.*;
import java.util.*;

class StreamGobbler extends Thread {
	InputStream is;
	String type;

	StreamGobbler(InputStream is, String type) {
		this.is = is;
		this.type = type;
	}

	public void run() {
		try {
			InputStreamReader isr = new InputStreamReader(is);
			BufferedReader br = new BufferedReader(isr);
			String line = null;
			while ((line = br.readLine()) != null)
				System.out.println(type + ">" + line);
		} catch (IOException ioe) {
			ioe.printStackTrace();
			System.exit(1);
		}
	}
}

abstract class Generator {
	abstract String generate(ListContent lc) throws Exception;
	abstract String generate(TextBlock tb) throws Exception;
	abstract String generate(LineBreak lb) throws Exception;
	abstract String generate(ImageContent ic) throws Exception;
	abstract String generate(LinkContent ic) throws Exception;
	abstract String generate(Paragraph p) throws Exception;
	abstract String generate(CodeSnippet cs) throws Exception;
	abstract String generate(TextContent tc) throws Exception;
	abstract String generate(Section section) throws Exception;
	abstract String generate(CodeSection section) throws Exception;


	String generateBase(ContentElement element) throws Exception {
		if (element instanceof TextBlock) {
			return generate((TextBlock)element);
		} else if (element instanceof ListContent) {
			return generate((ListContent)element);
		} else if (element instanceof TextContent) {
			return generate((TextContent)element);
		} else if (element instanceof LineBreak) {
			return generate((LineBreak)element);
		} else if (element instanceof ImageContent) {
			return generate((ImageContent)element);
		} else if (element instanceof Paragraph) {
			return generate((Paragraph)element);
		} else if (element instanceof CodeSnippet) {
			return generate((CodeSnippet)element);
		} else if (element instanceof CodeSection) {
			saveCode((CodeSection)element);	// <-- here be hacks
			return generate((CodeSection)element);
		} else if (element instanceof LinkContent) {
			return generate((LinkContent)element);
		} else if (element instanceof Section) {
			return generate((Section)element);
		}
		return null;
	}

	//****************************************************************************
	// code compiling
	//****************************************************************************

	private Map<String, String> mCodeFiles = new HashMap<String, String>();

	/**
	 * Must be called from the Generator implementation
	 */
	void initCode() {
		mCodeFiles.clear();
	}

	/**
	 * We call this one ourselves
	 */
	void saveCode(CodeSection cs) {
		//System.out.println("saving " + cs.getFilename());
		String source = mCodeFiles.get(cs.getFilename());
		if (source != null) {
			source += cs.getSource();
		} else {
			source = cs.getSource();
		}
		mCodeFiles.put(cs.getFilename(), source);
	}

	static int exec(String line, boolean showOutput, File dir) throws Exception {
		Process process = null;

		if (showOutput) {
			System.out.println("exec: " + line);
		}

		process = Runtime.getRuntime().exec("cmd /c " + line, null, dir);
		InputStream stdin = process.getInputStream();
		InputStream stderr = process.getErrorStream();

		StreamGobbler stdinGobbler = new StreamGobbler(stdin, "stdin: ");
		StreamGobbler stderrGobbler = new StreamGobbler(stderr, "stderr: ");
		stdinGobbler.start();
		stderrGobbler.start();
		process.waitFor();
		if (showOutput) {
			System.out.println("exec \"" + line + "\" returned " + process.exitValue());
		}
		return process.exitValue();
	}

	/**
	 * Must be called from the Generator implementation
	 */
	void compileCode() throws Exception {
		System.out.println("Compiling code...\n");
		Set<Map.Entry<String, String>> set = mCodeFiles.entrySet();
		Iterator<Map.Entry<String, String>> itr = set.iterator();
		while (itr.hasNext()) {
			Map.Entry<String, String> entry = itr.next();
			File file = new File(entry.getKey());
			String source = entry.getValue();

			//dump file
			{
				FileOutputStream fos = new FileOutputStream(file);
				fos.write(source.getBytes());
				fos.close();
			}
		}

		itr = set.iterator();
		while (itr.hasNext()) {
			Map.Entry<String, String> entry = itr.next();
			File file = new File(entry.getKey());
			if(!file.getName().endsWith(".h")) {
				//compile file, standard MoSync settings
				String mosyncdir = System.getenv("FULLPATH_MOSYNCDIR");
				
				if(mosyncdir == null)
					mosyncdir = System.getenv("MOSYNCDIR");
				
				String cmd = mosyncdir + "/bin/xgcc -g -S \"" +
					file.getAbsolutePath() + "\" -I" + mosyncdir + "/include";
				if (exec(cmd, true, null) != 0)
					throw new Exception("Compilation failed: " + file.getName());
			}
		}

		itr = set.iterator();
		while (itr.hasNext()) {
			Map.Entry<String, String> entry = itr.next();
			File file = new File(entry.getKey());
			if (!file.delete())
				throw new Exception("Could not delete file " + file.getName());
		}
	}
}

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

package com.mobilesorcery.mosync.docbook;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

class LaTeXGenerator extends Generator {

	private static LaTeXGenerator instance;

	@Override
	String generate(Section section) throws Exception {
		// TODO Auto-generated method stub
		StringBuffer output = new StringBuffer();
		
		if(!(section.getTitle() == null || section.getTitle().isEmpty())) {
			//System.out.println("outputting section: " + section.getTitle());
			output.append("\\section{"+section.getTitle()+"}\n");
		}
/*		if(!(section.getAuthor() == null || section.getAuthor().isEmpty())) {
			output.append("<h2 class=\"author\"> Written by: "+section.getAuthor()+"</h2></div>");
		}
*/		
		
		for(int i = 0; i < section.getElements().size(); i++) {
			output.append(generateBase(section.getElements().get(i)));
		}
		
		return output.toString();
	}
	
	
	static LaTeXGenerator getInstance() {
		if(instance == null)
			return new LaTeXGenerator();
		else
			return instance;
	}
	
	
	void generate(ArrayList<Doc> docs, File outputFolder, File templateFile) throws Exception {
		//System.out.println(generateMenu(docs));
		for(int i = 0; i < docs.size(); i++) {
			generate(docs.get(i), 
					new File(outputFolder.getAbsolutePath()+"/"+docs.get(i).getID()+".tex"),
					templateFile);
		}
		//System.out.println("Generation complete");
	}
	
	
	void generate(Doc doc, File outputFolder, File templateFile) throws Exception {
		
		try {//Retrieve template
			BufferedReader in = new BufferedReader(new FileReader(templateFile));
			StringBuffer buf = new StringBuffer();
			while(in.ready()) {
				buf.append(in.readLine()+"\n");
			}
			in.close();
			
			//System.out.println("Generating LaTeX file: "+outputFolder.getAbsolutePath());
			ArrayList<Section> sections = doc.getSections();
		
			StringBuffer output = new StringBuffer();
			
			String title = doc.getTitle();
			String author = doc.getAuthor();

			output.append("\\documentclass[a4paper]{article}\n");
			output.append("\\usepackage[latin1]{inputenc}\n");
			output.append("%\\usepackage[swedish]{babel}\n");
			output.append("\\usepackage{fancyhdr}\n");
			output.append("\\usepackage{graphicx}\n");
			output.append("\\usepackage{ulem}\n");
			output.append("\\title{"+title+"}\n");
			output.append("\\author{"+author+"}\n");
			output.append("\\begin{document}\n");
			output.append("\\maketitle\n");
			output.append("\\thispagestyle{empty}\n");
			output.append("\\newpage\n");
			output.append("\\tableofcontents\n");
			output.append("\\thispagestyle{empty}\n");
			output.append("\\newpage\n");
			output.append("\\setcounter{page}{1}\n");
			output.append("\\pagestyle{fancy}\n");
			
			for(int x=0; x < sections.size(); x++) {
				output.append(generate(sections.get(x)));
			}

			output.append("\\end{document}\n");
			
			FileWriter out = new FileWriter(outputFolder);
			out.write(output.toString());
			out.flush();
			out.close();
			//System.out.println("Generation complete");
		}
		catch (FileNotFoundException ioe) {
			ioe.printStackTrace();
		}
		catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}
	
	@Override
	String generate(ListContent lc) throws Exception {
		StringBuffer output = new StringBuffer();

		if(lc.getChilds().size() > 0) {
			if(lc.getOrdered())
				output.append("\\begin{enumerate}\n");
			else
				output.append("\\begin{itemize}");
		}
		
		for(int i = 0; i < lc.getChilds().size(); i++) {
			output.append("\\item ");
			output.append(generateBase(lc.getChilds().get(i)) + "\n");
		}
		
		if(lc.getChilds().size() > 0) {
			if(lc.getOrdered())
				output.append("\\end{enumerate}\n");
			else
				output.append("\\end{itemize}\n");
		}

		return output.toString();
	}

	@Override
	String generate(TextBlock tb) throws Exception {
		// TODO Auto-generated method stub
		StringBuffer buf = new StringBuffer();
		
		for(int i = 0; i < tb.getContents().size(); i++) {
			buf.append(generateBase(tb.getContents().get(i)).replace("_", "\\_"));
		}
		if(tb.getAttributes()== TextBlock.ATTR_BOLD) {
			buf.insert(0, "\\textbf{");
			buf.append("}");
		}
		else if(tb.getAttributes()== TextBlock.ATTR_ITALIC) {
			buf.insert(0, "\\textit{");
			buf.append("}");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_MONOSPACED) {
			buf.insert(0, "\\texttt{");
			buf.append("}");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_DEL) {
			buf.insert(0, "\\sout{");
			buf.append("}");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_EMPH) {
			buf.insert(0, "\\emph{");
			buf.append("}");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_SUB) {
			buf.insert(0, "$_{\\textrm{");
			buf.append("}}$");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_SUP) {
			buf.insert(0, "$^{\\textrm{");
			buf.append("}}$");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_UNDERLINED) {
			buf.insert(0, "\\underline{");
			buf.append("}");
		}
		
		return buf.toString();
	}

	@Override
	String generate(ImageContent ic) {
		//return null;
		//return "Image goes here: " + ic.getURL() + "\n";
		return "%\\includegraphics{" + ic.getURL() + "}\n";
	}

	@Override
	String generate(LinkContent ic) {
		return ic.getURL();
		//return null;
	}

	@Override
	String generate(Paragraph p) throws Exception {
		StringBuffer output = new StringBuffer();
		for(int i = 0; i < p.getBlocks().size(); i++) {
			String s = generateBase(p.getBlocks().get(i));
			output.append(i == 0 ? s.trim() : s);
		}
		output.append("\n");
		return output.toString();
	}

	@Override
	String generate(CodeSnippet cs) throws Exception {
		StringBuffer output = new StringBuffer();
		output.append("\\begin{verbatim}\n");
		output.append(cs.getSource());
		output.append("\\end{verbatim}\n");

		return output.toString();
	}
	
	@Override
	String generate(CodeSection cs) {
		StringBuffer output = new StringBuffer();
		output.append("\\begin{verbatim}\n");
		output.append(cs.getSource());
		output.append("\\end{verbatim}\n");

		return output.toString();
	}

	@Override
	String generate(LineBreak lb) {
		return "\n\n";
	}


	@Override
	String generate(TextContent tc) {
		return tc.getText();
	}
}

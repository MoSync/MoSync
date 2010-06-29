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

/**
 * MoSync Documentation Parser
 * XML + dokuwiki syntax based documentation
 *
 * Copyright © Mobile Sorcery
 *
 * Author: Aaron Chan
 *
 * HTMLGenerator.java - Module for generating HTML output
 *
 * Last edited: 07 September 2009
 *
 */

package com.mobilesorcery.mosync.docbook;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;

import javax.imageio.ImageIO;

import com.uwyn.jhighlight.renderer.XhtmlRendererFactory;

public class HTMLGenerator extends Generator {

	private static HTMLGenerator instance = null;

	private File outputFolder;

	private HTMLGenerator() {
		//System.out.println("==================================================");
		//System.out.println("HTML Generator initialized");
		//System.out.println("==================================================");
	}

	public static HTMLGenerator getInstance() {
		if(instance == null)
			instance = new HTMLGenerator();

		return instance;
	}

	// This is the root function, I think
	public void generate(DocFolder root, File outputFolder, File templateFile) throws Exception {
		ArrayList<DocFolder> children = root.getChildren();
		System.out.println("root has " + children.size() + " children!");
		if (templateFile.getName().contains("eclipse")) {
			generateEclipseHelpMenu(root, outputFolder, templateFile);
		}
		for(int i = 0; i < children.size(); i++) {
			generate(
				root,
				children.get(i),
				outputFolder,
				templateFile
			);
		}
		System.out.println("Compiling...");
		if(!System.getProperty("os.name").toLowerCase().contains("mac"))
			 compileCode();
		System.out.println("done");
	}

	public void generate(DocFolder root, DocFolder folder, File outputFolder, File templateFile) throws Exception {
		System.out.println("Generating folder: " + folder.getName());
		ArrayList<Doc> docs = folder.getDocs();
		ArrayList<DocFolder> children = folder.getChildren();

		generateMenuFramePage(root, outputFolder);

		for(int i = 0; i < docs.size(); i++) {
			System.out.println("Generating doc: " + docs.get(i).getID());
				generate(
					root,
					docs.get(i),
					new File(outputFolder.getAbsolutePath()+"/"+docs.get(i).getID()+".html"),
					templateFile,
					//generateMenu(root, docs.get(i))
					generateMenu(root, outputFolder)//generateMenuIFrameTag(root, docs.get(i))
				);
		}

		for(int i = 0; i < children.size(); i++)
			generate(
				root,
				children.get(i),
				outputFolder,
				templateFile
			);
	}



/*	void generateIndex(ArrayList<Doc> docs, File outputFolder, File templateFile) {

		File outputFile = new File(outputFolder.getAbsolutePath()+"/index.html");
		try {//Retrieve template
			BufferedReader in = new BufferedReader(new FileReader(templateFile));
			StringBuffer buf = new StringBuffer();
			while(in.ready()) {
				buf.append(in.readLine()+"\n");
			}
			in.close();
			// Put templateFile into String
			String result = buf.toString();

			//System.out.println("Generating HTML file: "+outputFile);
			buf = new StringBuffer();

			// Generate Links
			Doc doc;
			ArrayList<Section> sections;
			Section section;
			for(int i = 0; i < docs.size(); i++) {
				doc = docs.get(i);
				buf.append("<li>");
				buf.append("<a href=\""+doc.getID()+".html\">"+doc.getTitle()+"</a>\n");

				sections = doc.getSections();
				if(sections.size() > 0)
					buf.append("<ul>");
				for(int j = 0; j < sections.size(); j++) {
					section = sections.get(j);
					if(section.getTitle() != null && !section.getTitle().isEmpty()) {
						buf.append("<li>");
						buf.append("<a href=\""+doc.getID()+".html#"+section.getID()+"\">"+section.getTitle()+"</a>\n");
						buf.append("</li>");
					}
				}
				if(sections.size() > 0)
					buf.append("</ul>");

				buf.append("</li>");
			}
			// Get resulting links
			String output = buf.toString();

			// Replace templates
			result = result.replace("<%title%>", "Content Page");
			result = result.replace("<%author%>", "");
			result = result.replace("<%content%>", output.toString());
			result = result.replace("<%menu%>", "");

			FileWriter out = new FileWriter(outputFile);
			out.write(result);
			out.flush();
			out.close();
		}
		catch (FileNotFoundException ioe) {
			ioe.printStackTrace();
		}
		catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}
*/



//	void generateSubMenu(DocFolder folder, StringBuffer buf) {
//		ArrayList<Section> sections;
//		Section section;
//		ArrayList<Doc> docs = folder.getDocs();
//		ArrayList<DocFolder> children = folder.getChildren();
//
//		//buf.append("<a href=\"www.google.com\">GOOGLE/a>");
//		//buf.append("<li>");
//		buf.append("<li id=\""+folder.getName()+"\">");
//		buf.append(folder.getName());
//
//		buf.append("<ul>\n");
//		for(int i = 0; i < docs.size(); i++) {
//			//buf.append("<li id=\"" + docs.get(i).getID() + "\">");
//			//buf.append("<a href=\""+docs.get(i).getID()+".html\">"+docs.get(i).getTitle()+"</a>\n");
//			//buf.append("<a onclick=\"loadParent(\""+docs.get(i).getID()+".html\")\">"+docs.get(i).getTitle()+"</a>\n");
//
//			buf.append("<li id=\""+docs.get(i).getID()+"\"><a href=\""+docs.get(i).getID()+".html\">"+docs.get(i).getTitle()+"</a>");
//
//			Doc current = docs.get(i);
//			//if(docs.get(i) == current)
//			{
//				sections = current.getSections();
//				if(sections.size() > 0)
//					//buf.append("\n<ul id=\"" + docs.get(i).getID() + "\">\n");
//					buf.append("\n<ul>\n");
//				for(int j = 0; j < sections.size(); j++) {
//					section = sections.get(j);
//					if(section.getTitle() != null && !section.getTitle().isEmpty()) {
//						//buf.append("<li>");
//						//buf.append("<a href=\""+current.getID()+".html#"+section.getID()+"\">"+section.getTitle()+"</a>\n");
//						//buf.append("</li>");
//						buf.append("<li id=\""+section.getID()+"\" ><a href=\""+current.getID()+".html#"+section.getID()+"\">"+section.getTitle()+"</a></li>\n");
//					}
//				}
//				if(sections.size() > 0)
//					buf.append("</ul>\n");
//			}
//			buf.append("</li>\n");
//		}
//
//		for(int i = 0; i < children.size(); i++) {
//			generateSubMenu(children.get(i), buf);
//		}
//		buf.append("</ul>\n");
//		buf.append("</li>\n");
//	}

	void generateSubMenu(DocFolder folder, StringBuffer buf) {
		ArrayList<Section> sections;
		Section section;
		ArrayList<Doc> docs = folder.getDocs();
		ArrayList<DocFolder> children = folder.getChildren();

		//buf.append("<a href=\"www.google.com\">GOOGLE/a>");
		//buf.append("<li>");
		buf.append("<li id=\""+folder.getName()+"List\">");
		buf.append(folder.getName());

		buf.append("<ul>\n");
		for(int i = 0; i < docs.size(); i++) {
			//buf.append("<li id=\"" + docs.get(i).getID() + "\">");
			//buf.append("<a href=\""+docs.get(i).getID()+".html\">"+docs.get(i).getTitle()+"</a>\n");
			//buf.append("<a onclick=\"loadParent(\""+docs.get(i).getID()+".html\")\">"+docs.get(i).getTitle()+"</a>\n");
			buf.append("<li id=\""+docs.get(i).getID()+"List\" class=\"document-node\" onclick=\"loadParent('"+docs.get(i).getID()+".html')\">"+docs.get(i).getTitle());

			Doc current = docs.get(i);
			//if(docs.get(i) == current)
			{
				sections = current.getSections();
				if(sections.size() > 0)
					//buf.append("\n<ul id=\"" + docs.get(i).getID() + "\">\n");
					buf.append("\n<ul>\n");
				for(int j = 0; j < sections.size(); j++) {
					section = sections.get(j);
					if(section.getTitle() != null && !section.getTitle().isEmpty()) {
						//buf.append("<li>");
						//buf.append("<a href=\""+current.getID()+".html#"+section.getID()+"\">"+section.getTitle()+"</a>\n");
						//buf.append("</li>");
						buf.append("<li id=\""+section.getID()+"List\" class=\"section-node\" onclick=\"loadParent('"+current.getID()+".html#"+section.getID()+"')\">"+section.getTitle()+"</li>\n");
					}
				}
				if(sections.size() > 0)
					buf.append("</ul>\n");
			}
			buf.append("</li>\n");
		}

		for(int i = 0; i < children.size(); i++) {
			generateSubMenu(children.get(i), buf);
		}
		buf.append("</ul>\n");
		buf.append("</li>\n");
	}


	/**
	 * Creates a sub menu to replace a default sub menu in Eclipse 
	 * using a template file.
	 */
	void generateEclipseHelpSubMenuReplacement(DocFolder folder, File outputFolder,  File templateFile) throws Exception {
		ArrayList<Doc> docs = folder.getDocs();
		ArrayList<DocFolder> children = folder.getChildren();
		StringBuffer content = new StringBuffer();
		for (int i = 0; i < docs.size(); i++) {
			content.append("<li><div class=\"section\"><div class=\"info\"><a href=\"" + docs.get(i).getID() +
					".html\"><h1 class=\"title\">" + docs.get(i).getTitle() + "</h1></a><p></p></div></div></li>\n");
		}
		for (int i = 0; i < children.size(); i++) {
			content.append("<li><a href=\"" + getFileName(children.get(i), outputFolder) + "\"><h1 class=\"title\">" +
					children.get(i).getName() + "</h1></a><p></p></li>\n");
		}

		File outputFile = new File(outputFolder.getAbsolutePath()+"/" + getFileName(folder, outputFolder));
		try {//Retrieve template
			//System.out.println("generate: Reading...");
			BufferedReader in = new BufferedReader(new FileReader(templateFile));
			StringBuffer buf = new StringBuffer();
			while(in.ready()) {
				buf.append(in.readLine()+"\n");
			}
			in.close();

			//System.out.println("generate: replacing...");

			String result = buf.toString();

			// Replace templates
			result = result.replace("<%title%>", folder.getParent() != null ? folder.getName() : "MoSync");
			result = result.replace("<%author%>", "");
			result = result.replace("<%content%>", content.toString());
//			result = result.replace("<%menu%>", "");
			result = result.replace("<%navpath%>", makeNavPath(folder));

			//System.out.println("generate: writing...");

			FileWriter out = new FileWriter(outputFile);
			out.write(result);
			out.flush();
			out.close();
		}
		catch (FileNotFoundException ioe) {
			ioe.printStackTrace();
		}
		catch (IOException ioe) {
			ioe.printStackTrace();
		}

	}

	HashMap<DocFolder, String> linkNames = new HashMap<DocFolder, String>();

	/**
	 * Creates a unique filename for the files that are created to replace
	 * the default Eclipse sub menus. Keeps the name for subsequent calls for the same DocFolder
	 */
	String getFileName(DocFolder folder, File outputFolder) throws Exception  {
		if (linkNames.containsKey(folder)) {
			return linkNames.get(folder);
		}
		String path = outputFolder.getAbsolutePath();
		String baseName = folder.getName();
		baseName = baseName.replace(" ", "_");
		baseName = baseName.replace("+", "p");
		String fileName = baseName + "Menu.html";
		int number = 1;

		while (new File(path + "/" + fileName).exists()) {
			fileName = baseName  + number + "Menu.html";
			number++;
			if (number > 100) {
				throw new Exception("Cannot find a unique filename for an Eclipse menu with the name " + folder.getName());
			}
		}
		linkNames.put(folder, fileName);
		return fileName;
	}

	/**
	 * Generate the topics of the Eclipse help system
	 */
	void generateEclipseHelpSubMenu(DocFolder folder, StringBuffer buf, File outputFolder, File templateFile) throws Exception {
		ArrayList<Section> sections;
		Section section;
		ArrayList<Doc> docs = folder.getDocs();
		ArrayList<DocFolder> children = folder.getChildren();
		String fileName = getFileName(folder, outputFolder);
		buf.append("<topic label=\"" + folder.getName() + "\" href=\"docs/" + fileName + "\">\n");
		generateEclipseHelpSubMenuReplacement(folder, outputFolder, templateFile);
		for (int i = 0; i < docs.size(); i++) {
			buf.append("<topic label=\"" + docs.get(i).getTitle() + "\" href=\"docs/" + docs.get(i).getID() + ".html\">\n");

			Doc current = docs.get(i);
			sections = current.getSections();
			if (sections.size() > 0) {
				for (int j = 0; j < sections.size(); j++) {
					section = sections.get(j);
					if (section.getTitle() != null && !section.getTitle().isEmpty()) {
						buf.append("<topic label=\"" + section.getTitle() + "\" href=\"docs/" + current.getID() + ".html#" + section.getID() + "\" />\n");
					}
				}
			}
			buf.append("</topic>\n");
		}
		for (int i = 0; i < children.size(); i++) {
			generateEclipseHelpSubMenu(children.get(i), buf, outputFolder, templateFile);
		}
		buf.append("</topic>\n");
	}

	/**
	 * Generates a .toc file for the Eclipse help system
	 */
	void generateEclipseHelpMenu(DocFolder root, File outputFolder, File templateFile) throws Exception {
		StringBuffer buf = new StringBuffer();
		String fileName = getFileName(root, outputFolder);
		buf.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" +
				"<?NLS TYPE=\"org.eclipse.help.toc\"?>\n\n" +
				"<toc label=\"MoSync SDK\" topic=\"docs/"+ fileName + "\">\n");
		generateEclipseHelpSubMenuReplacement(root, outputFolder, templateFile);
		ArrayList<DocFolder> children = root.getChildren();
		for (int i = 0; i < children.size(); i++) {
			generateEclipseHelpSubMenu(children.get(i), buf, outputFolder, templateFile);
		}

		buf.append("</toc>");

		FileWriter out = new FileWriter(outputFolder.getAbsolutePath()+"/toc.xml");
		out.write(buf.toString());
		out.flush();
		out.close();
	}

	void generateMenuFramePage(DocFolder root, File outputFolder) throws Exception {
		StringBuffer buf = new StringBuffer();
		// Generate Links

		buf.append(
"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"+
"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">\n"+
"<head>\n"+
"<title>Contents</title>\n"+
"<meta http-equiv=\"Content-Type\" content=\"text/html;charset=iso-8859-1\" />\n" +
"<script type=\"text/javascript\" src=\"menu/simpletreemenu.js\">\n"+
"/***********************************************\n"+
"* Simple Tree Menu- © Dynamic Drive DHTML code library (www.dynamicdrive.com)\n"+
"* This notice MUST stay intact for legal use\n"+
"* Visit Dynamic Drive at http://www.dynamicdrive.com/ for full source code\n"+
"***********************************************/\n"+
"</script>\n"+
"<script type=\"text/javascript\">\n" +
"function loadParent(url) {\n" +
"window.parent.location.href = url;\n"+
"}\n"+
"</script>\n"+
"<link rel=\"stylesheet\" type=\"text/css\" href=\"menu/simpletree.css\" />\n"+
"</head>\n"+
"<body>\n");


		// Insert index URL;
		buf.append("<ul id=\"treemenu1\" class=\"treeview\">\n");
		//buf.append("<a href=\"index.html\">Content Page</a>\n");

		ArrayList<DocFolder> children = root.getChildren();
		for(int i = 0; i < children.size(); i++)
			generateSubMenu(children.get(i), buf);

		buf.append("</ul>\n");

		buf.append(
"<script type=\"text/javascript\">\n"+
"ddtreemenu.createTree(\"treemenu1\", false)\n"+
"</script>\n"+
"<p>\n" +
"	<a href=\"http://validator.w3.org/check?uri=referer\">" +
"<img src=\"http://www.w3.org/Icons/valid-xhtml10\" alt=\"Valid XHTML 1.0 Strict\" height=\"31\" width=\"88\" /></a>\n" +
"</p>\n" +
"</body>\n"+
"</html>\n");

		//return buf.toString();
		FileWriter out = new FileWriter(outputFolder.getAbsolutePath()+"/contents.html");
		out.write(buf.toString());
		out.flush();
		out.close();
	}

	String generateMenu(DocFolder root, File outputFolder) throws Exception {

		StringBuffer buf = new StringBuffer();
		// Insert index URL;
		buf.append("<ul id=\"treemenu1\" class=\"treeview\">\n");
		//buf.append("<a href=\"index.html\">Content Page</a>\n");

		ArrayList<DocFolder> children = root.getChildren();
		for(int i = 0; i < children.size(); i++)
			generateSubMenu(children.get(i), buf);

		buf.append("</ul>\n");
		return buf.toString();
	}

	String generateMenuIFrameTag(DocFolder root, Doc current) {
		return "<iframe id=\"contents\" name=\"contents\" align=\"left\" src=\"contents.html\" " +
		"onload=\"expand(frames['contents'].document.getElementById('" + current.getID() + "'),'" + current.getID() +"')\""+

		">Alternative text for browsers that do not understand IFrames.</iframe>"/* +
		"<script>" +
		"expand(frames['contents'].document.getElementById('" + current.getID() + "'),'" + current.getID() +"')"+
		"</script>"*/;
	}

	String makeNavPath(DocFolder folder) {
		ArrayList<String> parts = new ArrayList<String>();
		while (folder != null) {
			parts.add(folder.getName());
			folder = folder.getParent();
		}
		parts.remove(parts.get(parts.size() - 1));
		Collections.reverse(parts);

		String ret = "";

		for (int i = 0; i < parts.size(); i++) {
			ret += parts.get(i);
			if (i < parts.size() - 1) ret += " > ";
		}
		return ret;
	}

	String makeNavPath(Doc doc) {
		DocFolder parent = doc.getParent();
		ArrayList<String> parts = new ArrayList<String>();
		parts.add(doc.getTitle());

		while(parent != null) {
			parts.add(parent.getName());
			parent = parent.getParent();
		}

		parts.remove(parts.get(parts.size()-1));

		Collections.reverse(parts);

		String ret = "";

		for(int i = 0; i < parts.size(); i++) {
			ret += parts.get(i);
			if(i < parts.size() - 1) ret += " > ";
		}
		return ret;
	}

	public void generate(DocFolder root, Doc doc, File outputFile, File templateFile, String menu) throws Exception {

		try {//Retrieve template
			//System.out.println("generate: Reading...");
			BufferedReader in = new BufferedReader(new FileReader(templateFile));
			StringBuffer buf = new StringBuffer();
			while(in.ready()) {
				buf.append(in.readLine()+"\n");
			}
			in.close();

			//System.out.println("generate: appending...");

			//System.out.println("Generating HTML file: "+outputFile);
			ArrayList<Section> sections = doc.getSections();

			StringBuffer output = new StringBuffer();

			for(int x=0; x < sections.size(); x++) {
				//System.out.println("generate: generating section " + sections.get(x).getID());
				output.append(generate(sections.get(x)));
			}

			//System.out.println("generate: replacing...");

			String result = buf.toString();

			// Replace templates
			result = result.replace("<%title%>", doc.getTitle());
			result = result.replace("<%author%>", "Written by: "+doc.getAuthor());
			result = result.replace("<%content%>", output.toString());
			result = result.replace("<%menu%>", menu);
			result = result.replace("<%navpath%>", makeNavPath(doc));

			//System.out.println("generate: writing...");

			FileWriter out = new FileWriter(outputFile);
			out.write(result);
			out.flush();
			out.close();
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
				output.append("<ol>"+"\n");
			else
				output.append("<ul>"+"\n");
		}

		for(int i = 0; i < lc.getChilds().size(); i++) {

			output.append("<li>");
			output.append(generateBase(lc.getChilds().get(i)));
			output.append("</li>"+"\n");
		}


		if(lc.getChilds().size() > 0) {
			if(lc.getOrdered())
				output.append("</ol>"+"\n");
			else
				output.append("</ul>"+"\n");
		}

		return output.toString();
	}

	@Override
	String generate(TextBlock tb) throws Exception {
		// TODO Auto-generated method stub
		StringBuffer buf = new StringBuffer();

		for(int i = 0; i < tb.getContents().size(); i++) {
			buf.append(generateBase(tb.getContents().get(i)));
		}

		//output tag for element
		if(tb.getAttributes()== TextBlock.ATTR_BOLD) {
			buf.insert(0, "<b>");
			buf.append("</b>");
		}
		else if(tb.getAttributes()== TextBlock.ATTR_ITALIC) {
			buf.insert(0, "<i>");
			buf.append("</i>");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_MONOSPACED) {
			buf.insert(0, "<tt>");
			buf.append("</tt>");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_SUB) {
			buf.insert(0, "<sub>");
			buf.append("</sub>");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_SUP) {
			buf.insert(0, "<sup>");
			buf.append("</sup>");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_DEL) {
			buf.insert(0, "<del>");
			buf.append("</del>");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_EMPH) {
			buf.insert(0, "<em>");
			buf.append("</em>");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_QUOTE) {
			buf.insert(0, "<quote>");
			buf.append("</quote>");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_NOTE) {
			buf.insert(0, "<note>");
			buf.append("</note>");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_UNDERLINED) {
			buf.insert(0, "<em class=\"u\">");
			buf.append("</em>");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_H3) {
			buf.insert(0, "<h3>");
			buf.append("</h3>");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_H4) {
			buf.insert(0, "<h4>");
			buf.append("</h4>");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_H5) {
			buf.insert(0, "<h5>");
			buf.append("</h5>");
		}
		else if(tb.getAttributes() == TextBlock.ATTR_HR) {
			buf.insert(0, "<hr/>");
		}

		return buf.toString();
	}

	@Override
	String generate(ImageContent ic) {
		// Create Image file on location if it is not on internet
		if(ic.getBImage() != null) {
			try {
		        //System.out.println("=============================Create Internal Image=========================");
				BufferedImage bi = ic.getBImage(); // retrieve image
		        File outputfile = new File(outputFolder.getAbsolutePath()+"/"+ic.getURL());
		        if(!outputfile.canWrite())  {
		        	outputfile.mkdirs();
		        }

		        String ext =  outputfile.getName().substring(outputfile.getName().lastIndexOf(".")+1);
		        ImageIO.write(bi, ext, outputfile);
		    }
			catch (IOException e) {
				//System.out.println(e.getMessage());
		    }
		}

		StringBuffer output = new StringBuffer();
		output.append("<br/><img src=\""+ic.getURL()+"\"");

		if(!ic.getWidth().isEmpty()) {
			output.append(" width=\""+ic.getWidth()+"\"");
			if(!ic.getHeight().isEmpty()) {
				output.append(" height=\""+ic.getHeight()+"\"");
			}
		}

		if(!ic.getAlt().isEmpty()) {
			output.append(" alt=\""+ic.getAlt()+"\"");
		}

		output.append("/><br/>");
		return output.toString();
	}

	@Override
	String generate(LinkContent lc) throws Exception {
		// TODO Auto-generated method stub
		StringBuffer output = new StringBuffer();

		String url = lc.getURL();
		//System.out.println("URL :"+url);
		if(lc.getType() == LinkContent.LINK_INTERNAL) {
			String docID = url.substring(0, url.indexOf("#"));
			String sectID = url.substring(url.indexOf("#"));

			url = docID+".html"+sectID;
		}


		output.append("<a href=\""+url+"\">");
		ArrayList<ContentElement> elements = lc.getElements();
		for(int i = 0; i < elements.size(); i++) {
			output.append(generateBase(elements.get(i)));
		}
		output.append("</a>");
		return output.toString();
	}

	@Override
	String generate(Paragraph p) throws Exception {
		// TODO Auto-generated method stub
		StringBuffer output = new StringBuffer();
		output.append("<p>"+"\n");
		for(int i = 0; i < p.getBlocks().size(); i++) {
			//System.out.println("BLOCK: "+p.getBlocks().get(i).toString());
			output.append(generateBase(p.getBlocks().get(i)));
		}
		output.append("</p>"+"\n");
		return output.toString();
	}

	@Override
	String generate(LineBreak lb) {
		// TODO Auto-generated method stub
		return "<br/>";
	}

	String genCode(String code) {
		try{
			//remove empty first line, if any
			if(code.charAt(0) == '\n')
				code = code.substring(1);

			return "</p><div class=\"code\">"+
			XhtmlRendererFactory.getRenderer("cpp")
			.highlight("temp.cpp", code, "ASCII", true)
			+"</div><p>";
		}
		catch(IOException ioe) {
			ioe.printStackTrace();
			return "";
		}
	}

	@Override
	String generate(CodeSection cs) {
		return genCode(cs.getSource());
	}

	@Override
	String generate(CodeSnippet cs) throws Exception {
		return genCode(cs.getSource());
	}

	@Override
	String generate(Section section) throws Exception {
		// TODO Auto-generated method stub
		StringBuffer output = new StringBuffer();

		output.append("<div class=\"section\">");
		if(!(section.getTitle() == null || section.getTitle().isEmpty())) {
			output.append("<div class=\"info\"><h1 class=\"title\">"+section.getTitle()+"</h1></div>");
		}

		// Create anchor
		output.append("<a name=\""+section.getID()+"\"></a>");

		for(int i = 0; i < section.getElements().size(); i++) {
			//System.out.println("genSec: element " + i + ": " + section.getElements().get(i).toString());
			output.append(generateBase(section.getElements().get(i)));
		}
		//System.out.println("genSec: elements done");
		output.append("</div><!-- /div section -->");

		return output.toString();
	}

	@Override
	String generate(TextContent tc) {
		// TODO Auto-generated method stub
		return tc.getText();
	}
}

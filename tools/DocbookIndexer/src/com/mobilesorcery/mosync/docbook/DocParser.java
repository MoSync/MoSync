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
 * Author: Aaron Chan, Patrick Broman
 *
 * DocParser.java
 *
 * Last edited: 07 September 2009
 *
 */

package com.mobilesorcery.mosync.docbook;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileFilter;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

import javax.imageio.ImageIO;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class DocParser {

	static final String root = "";
	private static String targetTemplate;
	public static void main(String[] args) throws Exception {
		//create an instance
		if (args.length != 0 && args[0].equalsIgnoreCase("eclipse")){
			System.out.println("Generating documentation for the eclipse help system");
			targetTemplate = "template/templateforeclipse.html";
		} else {
			targetTemplate = "template/template.html";
		}
		
		DocParser gen = new DocParser();

		// Parse each file in the folder provided on the
		// command.
		// Get files;
		File dir = new File(root+"input");

		(new File(root+"output")).mkdir();


		//System.out.println("Parsing Directory: "+dir.getAbsolutePath());
		gen.runGenerator(dir);
		System.out.println("Success!");
	}

	public String stripIndexPrefix(String s) {
		int idx = s.indexOf('_');
		String ret = idx != -1 ? s.substring(idx+1) : s;
		//System.out.println("**** ATTENZIONE!!! : " + ret);
		return ret;
	}


	public void processDirectory(File dir, DocFolder folder) throws Exception {
		//System.out.println("processDirectory " + dir.getName());

		File[] files = dir.listFiles();

		//System.out.println("Dir " + dir.getAbsolutePath() + " has " + files.length + " files.");

		for (int i = 0; i < files.length; i++) { // Each XML source in Directory
			//System.out.println("Parsing: "+files[i].getAbsolutePath());
			//parse the xml file and get the dom object

			if(files[i].isDirectory()) {
				DocFolder child = new DocFolder(stripIndexPrefix(files[i].getName()));
				processDirectory(files[i], child);
				if(child.getChildren().size() > 0 || child.getDocs().size() > 0)
					folder.addChild(child);
			} else {
				try {
					if(files[i].getName().endsWith(".xml"))
						folder.addDoc(parseDocument(parseXmlFile(files[i])));
				}
				catch(Exception e) {
					//e.printStackTrace();
					System.err.println("Error parsing document \""+files[i].getName()+"\": " +
						e.getClass().toString() + " \""+e.getMessage()+"\"");
					throw e;
				}
			}
		}
		//System.out.println("processDirectory " + dir.getName() + " done");
	}


/*	public void makeDirs(File outputDir, DocFolder root) {
			ArrayList<DocFolder> children = root.getChildren();
			for(int i = 0; i < children.size(); i++) {
				File newFile = new File(outputDir.getAbsolutePath() + "/" + children.get(i).getName());
				newFile.mkdir();
				makeDirs(newFile, children.get(i));
			}
	} */

	public void runGenerator(File dir) throws Exception {
		//System.out.println("Generator running: ");

		DocFolder rootFolder = new DocFolder(dir.getName());

		// The list of files can also be retrieved as File objects

		processDirectory(dir, rootFolder);

		File file = new File(root+"output/");
		//makeDirs(file, rootFolder);

		File template = new File(root + targetTemplate);
		HTMLGenerator htmlGen = HTMLGenerator.getInstance();
		//htmlGen.generate(mergesort(documents), file, template);

		htmlGen.generate(rootFolder, file, template);
	}

	public Document parseXmlFile(File file) throws Exception {
		System.out.println("Parsing \"" + file.getName() + "\"");
		//get the factory
		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		//Using factory get an instance of document builder
		DocumentBuilder db = dbf.newDocumentBuilder();

		//parse using builder to get DOM representation of the XML file
		Document d = db.parse(file);
		return d;
	}

	public Doc parseDocument(Document dom) throws Exception {
		// result
		mCodeFilename = null;

		//Create Document object for each file
		Element docEle = dom.getDocumentElement();
		File file = new File(docEle.getBaseURI());

		String[] areas = docEle.getAttribute("area").split(",");
		String[] keywords = docEle.getAttribute("keyword").split(",");

		Doc doc = new Doc(file.getName().substring(0, file.getName().indexOf(".xml")),
				docEle.getAttribute("title"), docEle.getAttribute("author"), areas, keywords);
		mSnippets = doc.getSnippets();

		NodeList childs = docEle.getChildNodes();
		for(int i = 0; i < childs.getLength(); i++) {
			Node node = childs.item(i);
			if(node.getNodeType() == Node.ELEMENT_NODE) {
				doc.addSection(parseSection(node));
			}
		}

		return doc;
	}

	private Section parseSection(Node sectionNode) throws Exception {
		NodeList nl = sectionNode.getChildNodes();
		Section section = null;

		if(sectionNode.hasAttributes()) {
			NamedNodeMap attrs = sectionNode.getAttributes();
			String id = null;
			String title = null;

			if(attrs.getNamedItem("id") != null) {
				id = attrs.getNamedItem("id").getNodeValue();
			}
			if(attrs.getNamedItem("title") != null) {
				title = attrs.getNamedItem("title").getNodeValue();
			}

			section = new Section(id, title);
		}
		else {
			section = new Section();
		}

		for(int i = 0; i < nl.getLength(); i++) {
			Node node = nl.item(i);
			if(node.getNodeType() == Node.TEXT_NODE ||
					node.getNodeType() == Node.CDATA_SECTION_NODE
			) {
				section.addContentElement(parseTextNode(node));
			}
			else if(node.getNodeType() == Node.ELEMENT_NODE) {
				section.addContentElement(parseSection(node));
			}
		}

		return section;
	}

	private ArrayList<ContentElement> parseTextNode(Node textNode) throws Exception {
		//System.out.println("Parsing Text");

		ArrayList<ContentElement> elements = new ArrayList<ContentElement>();

		StringTokenizer lines = new StringTokenizer(textNode.getTextContent(), "\n");

		String block = "";
		String line = "";

		while(lines.hasMoreTokens()) {
			line = lines.nextToken();

			if(line.trim().length() > 1) {
				if(line.trim().charAt(0) == '-' || line.trim().charAt(0) == '*'
				&& line.trim().charAt(1) != '*' && line.trim().charAt(1) != '-'
				) {
					//System.out.println("Block: "+block);
					elements.addAll(parseText(block));
					block = "";

					while(true) {
						if(line.trim().isEmpty() || (line.trim().charAt(0) != '-'
							&& line.trim().charAt(0) != '*')
							&& line.trim().charAt(1) != '*' && line.trim().charAt(1) != '-')
						{
							String[] arrays = block.split("\n");
							if(arrays.length > 0) {
								//System.out.println("||"+block+"||");
								elements.add(parseList(arrays));
							}
							block = "";
							break;
						}
						else {
							block += line+"\n";
							line = lines.nextToken();
						}
					}
				}
			}
			block += line+"\n";
		}

		elements.add(new Paragraph(parseText(block)));

		return elements;

	}

	private ListContent parseList(String[] contents) throws Exception {
		//System.out.println("Parsing List: ");
		for(int i = 0; i < contents.length; i++) {
			//System.out.println(contents[i]);
		}
		String line ;
		int lastPos = -1;
		ListContent newList = null;
		if(contents.length > 0) {
			if(contents[0].trim().charAt(0) == '-') {
				lastPos = contents[0].indexOf("-");
				newList = new ListContent(false);
			}
			else if(contents[0].trim().charAt(0) == '*') {
				lastPos = contents[0].indexOf("*");
				newList = new ListContent(true);
			}
		}

		for(int i = 0; i < contents.length; i++) {
			line = contents[i];

			if(line.trim().charAt(0) == '-') { // New Sub List
				if(lastPos < line.indexOf("-")) {
						String[] dest = new String[contents.length - (i)];
						System.arraycopy(contents, (i), dest, 0, contents.length-(i));
						ListContent list = parseList(dest);
						newList.addList(list);
						i = i + list.getChilds().size()-1;
				}
				else if(lastPos > line.indexOf("-")) {// Use previous list
					return newList;
				}
				else if(lastPos == line.indexOf("-")){// Same list
					newList.addContentElements(parseText(line.substring(line.indexOf("-")+1)));
					lastPos = line.indexOf("-");
				}
			}
			else if(line.trim().charAt(0) == '*') { // New Sub List
				if(lastPos < line.indexOf("*")) {
						String[] dest = new String[contents.length - (i)];
						System.arraycopy(contents, (i), dest, 0, contents.length-(i));
						ListContent list = parseList(dest);
						newList.addList(list);
						//System.out.println("no. of child: "+list.getChilds().size());
						i = i + list.getChilds().size()-1;
				}
				else if(lastPos > line.indexOf("*")) {// Use previous list
					return newList;
				}
				else if(lastPos == line.indexOf("*")){// Same list
					newList.addContentElements(parseText(line.substring(line.indexOf("*")+1)));
					//System.out.println("Added: "+line.substring(line.indexOf("*")+1));
					lastPos = line.indexOf("*");
				}
			}
		}

		return newList;
	}

	private String mCodeFilename;
	private Map<String, String> mSnippets;

	private ArrayList<ContentElement> parseText(String content) throws Exception {
		ArrayList<ContentElement> elements = new ArrayList<ContentElement>();
		ArrayList<ContentElement> results = new ArrayList<ContentElement>();
		StringBuffer buffer = new StringBuffer();
		Stack<String> state = new Stack<String>();
		int snippetStartIndex = -1;
		String snippetId = null;
		boolean newPara = false, skipNewLine = false;
		for(int i = 0; i < content.length(); i++) {
			if (skipNewLine) {
				skipNewLine = false;
				if (content.charAt(i) == '\n')
					continue;
			}
			buffer.append(content.charAt(i));
			//System.out.println("buffer: "+buffer);
			//System.out.println("state: "+state);
			if(!buffer.toString().endsWith("\n") && !buffer.toString().endsWith(" ")) {
				newPara = false;
			}

			//**********************************************************************
			// usesnippet
			//**********************************************************************
			if (buffer.toString().endsWith("[usesnippet")) {
				if (state.empty()) {
					state.add("[usesnippet");
					//Adding element before usesnippet
					elements.add(new TextBlock(
						buffer.toString().substring(0, buffer.length() - "[usesnippet".length()),
						TextBlock.ATTR_NORM));
					buffer.delete(0, buffer.length());
				}
			} else if (buffer.toString().endsWith("id=") &&
				!state.empty() && state.peek().equals("[usesnippet"))
			{
					state.pop();
					state.add("usesnippet id=");
			}
			else if (buffer.toString().endsWith("]") &&
				!state.empty() && state.peek().equals("usesnippet id="))
			{
				//get id
				String string = buffer.toString();
				int index1 = string.indexOf('"') + 1;
				snippetId = string.substring(index1, string.indexOf('"', index1));
				System.out.println("Used snippet id: " + snippetId);

				//String source = mSnippets.get(snippetId);
				//if(source == null)
					//throw new Exception("Unknown snippet: " + snippetId);
				elements.add(new CodeSnippet(mSnippets, snippetId));
				buffer.delete(0, buffer.length());
				state.pop();
			}
				//**********************************************************************
				// code
				//**********************************************************************
			else if (buffer.toString().endsWith("[code")) {
				if (state.empty()) {
					state.add("[code");
					//System.out.println("Buffer in code: "+buffer);
					//System.out.println("State in code: "+state);
					//System.out.println("added normal in code: "+buffer.toString().substring(0, buffer.length()-6));
					//Adding element before Code
					elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 5), TextBlock.ATTR_NORM));
					buffer.delete(0, buffer.length());
				}
			} else if (buffer.toString().endsWith("filename=") &&
				!state.empty() && state.peek().equals("[code")) {
				state.pop();
				state.add("filename=");
			} else if (buffer.toString().endsWith("]") &&
				!state.empty() && (state.peek().equals("[code") || state.peek().equals("filename="))) {
				if (state.peek().equals("filename=")) {
					//get filename
					String string = buffer.toString();
					int index1 = string.indexOf('"') + 1;
					mCodeFilename = string.substring(index1, string.indexOf('"', index1));
					System.out.println("Found code filename: " + mCodeFilename);
				} else {
					//System.out.println("Code section without filename. Using previous: " + mCodeFilename);
				}
				if (mCodeFilename == null)
					throw new Exception("Code section without filename!");

				buffer.delete(0, buffer.length());

				state.pop();
				state.add("[code]");

				//**********************************************************************
				// snippet
				//**********************************************************************
			} else if (buffer.toString().endsWith("[snippet")) {
				if (!state.empty() && state.peek().equals("[code]")) {
					//System.out.println("Found snippet");
					state.add("[snippet");
					snippetStartIndex = buffer.length() - "[snippet".length();
				}
			} else if (buffer.toString().endsWith("id=") &&
				!state.empty() && state.peek().equals("[snippet")) {
				//System.out.println("Found snippet id");
				state.pop();
				state.add("snippet id=");
			} else if (buffer.toString().endsWith("]") &&
				!state.empty() && state.peek().equals("snippet id=")) {
				//get filename
				String string = buffer.substring(snippetStartIndex);
				int index1 = string.indexOf('"') + 1;
				snippetId = string.substring(index1, string.indexOf('"', index1));
				System.out.println("Found snippet id: " + snippetId);

				buffer.delete(snippetStartIndex, buffer.length());
				state.pop();
				state.add("[snippet]");
				skipNewLine = true;
			} else if (buffer.toString().endsWith("[/snippet]")) {
				if (!state.empty() && state.peek().equals("[snippet]")) {
					buffer.delete(buffer.length() - "[/snippet]".length(), buffer.length());
					mSnippets.put(snippetId, buffer.substring(snippetStartIndex));
					state.pop();
					skipNewLine = true;
				}

				//**********************************************************************
				// end of code
				//**********************************************************************
			} else if (buffer.toString().endsWith("[/code]")) {
				if (!state.empty() && state.peek().equals("[code]")) {
					state.pop();
					if (state.empty()) {
						elements.add(new CodeSection(
							buffer.toString().substring(0, buffer.toString().length() - 7), mCodeFilename));
						//System.out.println("added code: "+buffer.toString().substring(0, buffer.toString().length()-7));
						buffer.delete(0, buffer.length());
					}
				}
			} else if (state.empty() || !state.peek().equals("[code]")) {
				//**********************************************************************
				// hyperlinks
				//**********************************************************************

				// Used to detect the end of plain text url
				if (buffer.toString().endsWith(" ")) {
					if (!state.empty()) {
						if (state.peek().equals("http://")) {
							state.pop();
							if (state.empty()) {
								elements.add(new LinkContent("http://" + buffer.toString(), new TextBlock("http://" + buffer.toString(), TextBlock.ATTR_NORM), LinkContent.LINK_EXTERNAL));
								//System.out.println("Added link element: " + buffer.toString());
								buffer.delete(0, buffer.length());
							}
						} else if (state.peek().equals("www.")) {
							state.pop();
							if (state.empty()) {
								elements.add(new LinkContent("http://www." + buffer.toString(), new TextBlock("www." + buffer.toString(), TextBlock.ATTR_NORM), LinkContent.LINK_EXTERNAL));
								//System.out.println("Added link element: " + buffer.toString());
								buffer.delete(0, buffer.length());
							}
						}
					}
				} else
					//**********************************************************************
					// paragraphs
					//**********************************************************************

					// To detect and insert new paragraph when there is two consecutive \n
					if (buffer.toString().endsWith("\n")) {
						if (newPara) {
							//if(buffer.toString().substring(0, buffer.length()-1).trim().endsWith("\n")) {
							//System.out.println("======================================NEW PARA=====================");
							elements.add(new TextBlock(buffer.toString(), TextBlock.ATTR_NORM));
							results.add(new Paragraph(elements));
							elements.removeAll(elements);
							buffer.delete(0, buffer.length());
							newPara = false;
						} else
							newPara = true;
					}

						//**********************************************************************
						// images
						//**********************************************************************
					else if (buffer.toString().endsWith("{{")) {
						if (state.empty()) {
							state.add("{{");
							//System.out.println("Buffer in image: "+buffer);
							//System.out.println("State in image: "+state);
							//System.out.println("added normal in image: "+buffer.toString().substring(0, buffer.length()-2));
							//Adding element before image
							elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 2), TextBlock.ATTR_NORM));
							buffer.delete(0, buffer.length());
						}
					} else if (buffer.toString().endsWith("}}")) {
						if (state.empty() || state.peek().equals("{{")) {
							state.pop();
							if (state.empty()) {
								elements.add(parseImage(buffer.toString().substring(0, buffer.toString().length() - 2)));
								//System.out.println("added image: "+buffer.toString().substring(0, buffer.toString().length()-2));
								buffer.delete(0, buffer.length());
							}
						}
					} else if (buffer.toString().endsWith("[[")) {
						if (state.empty()) {
							state.add("[[");
							//System.out.println("Buffer in link: "+buffer);
							//System.out.println("state in link: "+state);
							//System.out.println("added normal in link: "+buffer.toString().substring(0, buffer.length()-2));
							//Adding element before image
							elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 2), TextBlock.ATTR_NORM));
							buffer.delete(0, buffer.length());
						}
					} else if (buffer.toString().endsWith("]]")) {
						if (state.empty() || state.peek().equals("[[")) {
							state.pop();
							if (state.empty()) {
								elements.add(parseLink(buffer.toString().substring(0, buffer.toString().length() - 2)));
								//System.out.println("added link: "+buffer.toString().substring(0, buffer.toString().length()-2));
								buffer.delete(0, buffer.length());
							}
						}
					} else if (buffer.toString().endsWith("((")) {
						if (state.empty()) {
							state.add("((");
							//System.out.println("Buffer in link: "+buffer);
							//System.out.println("state in link: "+state);
							//System.out.println("added normal in link: "+buffer.toString().substring(0, buffer.length()-2));
							//Adding element before image
							elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 2), TextBlock.ATTR_NORM));
							buffer.delete(0, buffer.length());
						}
					} else if (buffer.toString().endsWith("))")) {
						if (!state.empty() && state.peek().equals("((")) {
							state.pop();
							if (state.empty()) {
								elements.add(new TextBlock(
										parseText(buffer.toString().substring(0, buffer.toString().length() - 2)
												), TextBlock.ATTR_NOTE)
								);
								//System.out.println("added note: "+buffer.toString().substring(0, buffer.toString().length()-2));
								buffer.delete(0, buffer.length());
							}
						}
					}
						// Parsing Quotes
					else if (buffer.toString().endsWith("<<")) {
						if (state.empty()) {
							state.add("<<");
							//System.out.println("Buffer in link: "+buffer);
							//System.out.println("state in link: "+state);
							//System.out.println("added normal in link: "+buffer.toString().substring(0, buffer.length()-2));
							//Adding element before image
							elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 2), TextBlock.ATTR_NORM));
							buffer.delete(0, buffer.length());
						}
					} else if (buffer.toString().endsWith(">>")) {
						if (!state.empty() && state.peek().equals("<<")) {
							state.pop();
							if (state.empty()) {
								elements.add(new TextBlock(
										parseText(buffer.toString().substring(0, buffer.toString().length() - 2)
												), TextBlock.ATTR_QUOTE)
								);
								//System.out.println("added quote: "+buffer.toString().substring(0, buffer.toString().length()-2));
								buffer.delete(0, buffer.length());
							}
						}
					} else if (buffer.toString().endsWith("<del>")) {
						if (state.empty()) {
							state.add("<del>");
							//System.out.println("added normal in del: "+buffer.toString().substring(0, buffer.length()-5));
							elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 5), TextBlock.ATTR_NORM));
							buffer.delete(0, buffer.length());
						}
					} else if (buffer.toString().endsWith("</del>")) {
						if (state.empty() || state.peek().equals("<del>")) {
							state.pop();
							//System.out.println(state.size());
							if (state.empty()) {
								elements.add(new TextBlock(
										parseText(buffer.toString().substring(0, buffer.length() - 6))
										, TextBlock.ATTR_DEL)
								);
								//System.out.println("added deleted: "+buffer.toString().substring(0, buffer.length()-6));
								buffer.delete(0, buffer.length());
							}
						}
					} else if (buffer.toString().endsWith("<sub>")) {
						//System.out.println("State in sub: "+state);
						if (state.empty()) {
							state.add("<sub>");
							//System.out.println("added normal in  sub: "+buffer.toString().substring(0, buffer.length()-5));
							elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 5), TextBlock.ATTR_NORM));
							buffer.delete(0, buffer.length());
						}
					} else if (buffer.toString().endsWith("</sub>")) {
						if (state.empty() || state.peek().equals("<sub>")) {
							state.pop();
							//System.out.println("State in sub end"+state);
							if (state.empty()) {
								elements.add(new TextBlock(
										parseText(buffer.toString().substring(0, buffer.length() - 6))
										, TextBlock.ATTR_SUB)
								);
								//System.out.println("added subbed: "+buffer.toString().substring(0, buffer.length()-6));
								buffer.delete(0, buffer.length());
							}
						}
					} else if (buffer.toString().endsWith("<sup>")) {
						//System.out.println("State in sub: "+state);
						if (state.empty()) {
							state.add("<sup>");
							//System.out.println("added normal in  sup: "+buffer.toString().substring(0, buffer.length()-5));
							elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 5), TextBlock.ATTR_NORM));
							buffer.delete(0, buffer.length());
						}
					} else if (buffer.toString().endsWith("</sup>")) {
						if (state.empty() || state.peek().equals("<sup>")) {
							state.pop();
							//System.out.println("State in sub end"+state);
							if (state.empty()) {
								elements.add(new TextBlock(
										parseText(buffer.toString().substring(0, buffer.length() - 6))
										, TextBlock.ATTR_SUP)
								);
								//System.out.println("added supped: "+buffer.toString().substring(0, buffer.length()-6));
								buffer.delete(0, buffer.length());
							}
						}
					} else if (buffer.toString().endsWith("<em>")) {
						//System.out.println("State in em: "+state);
						if (state.empty()) {
							state.add("<em>");
							//System.out.println("added normal in  em: "+buffer.toString().substring(0, buffer.length()-4));
							elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 4), TextBlock.ATTR_NORM));
							buffer.delete(0, buffer.length());
						}
					} else if (buffer.toString().endsWith("</em>")) {
						if (state.empty() || state.peek().equals("<em>")) {
							state.pop();
							//System.out.println("State in sub end"+state);
							if (state.empty()) {
								elements.add(new TextBlock(
										parseText(buffer.toString().substring(0, buffer.length() - 5))
										, TextBlock.ATTR_EMPH)
								);
								//System.out.println("added subbed: "+buffer.toString().substring(0, buffer.length()-5));
								buffer.delete(0, buffer.length());
							}
						}
					}
						// BOLD
					else if (buffer.toString().endsWith("**")) {
						if (state.empty()) {
							elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 2), TextBlock.ATTR_NORM));
							//System.out.println("added normal: "+buffer.toString().substring(0, buffer.length()-2));
							buffer.delete(0, buffer.length());
							state.add("**");
						} else if (state.peek().equals("**")) {
							state.pop();
							if (state.empty()) {
								elements.add(new TextBlock(
										parseText(buffer.toString().substring(0, buffer.length() - 2))
										, TextBlock.ATTR_BOLD)
								);
								//System.out.println("added bold: "+buffer.toString().substring(0, buffer.length()-2));
								buffer.delete(0, buffer.length());
							}
						}
					}
						// MONOSPACED
					else if (buffer.toString().endsWith("''")) {
						if (state.empty()) {
							elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 2), TextBlock.ATTR_NORM));

							//System.out.println("added normal: "+buffer.toString().substring(0, buffer.length()-2));
							buffer.delete(0, buffer.length());
							state.add("''");
						} else if (state.peek().equals("''")) {
							state.pop();
							if (state.empty()) {
								elements.add(new TextBlock(parseText(buffer.toString().substring(0, buffer.length() - 2))
										, TextBlock.ATTR_MONOSPACED));
								//System.out.println("added mono: "+buffer.toString().substring(0, buffer.length()-2));
								buffer.delete(0, buffer.length());
							}
						}
					} else if (buffer.toString().endsWith("__")) {
						if (state.empty()) {
							elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 2), TextBlock.ATTR_NORM));

							//System.out.println("added normal: "+buffer.toString().substring(0, buffer.length()-2));
							buffer.delete(0, buffer.length());
							state.add("__");
						} else if (state.peek().equals("__")) {
							state.pop();
							if (state.empty()) {
								elements.add(new TextBlock(parseText(buffer.toString().substring(0, buffer.length() - 2))
										, TextBlock.ATTR_UNDERLINED));
								//System.out.println("added underline: "+buffer.toString().substring(0, buffer.length()-2));
								buffer.delete(0, buffer.length());
							}
						}
					} else if (buffer.toString().endsWith("\\\\")) {
						if (content.length() > i + 1 && content.charAt(i + 1) == '\n') {
							if (state.empty()) {
								i++;
								buffer.append(content.charAt(i));
								elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 3), TextBlock.ATTR_NORM));
								//System.out.println("added normal: "+buffer.toString().substring(0, buffer.length()-3));
								elements.add(new LineBreak());
								//System.out.println("added linebreak");
								buffer.delete(0, buffer.length());
							}
						}
					}

					// HR
					else if (buffer.toString().endsWith("======")) {
						if (state.empty()) {
							elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 6), TextBlock.ATTR_NORM));
							//System.out.println("added normal: "+buffer.toString().substring(0, buffer.length()-6));
							elements.add(new TextBlock(
									parseText(buffer.toString().substring(0, buffer.length() - 6))
									, TextBlock.ATTR_HR)
							);
							//System.out.println("added hr: "+buffer.toString().substring(0, buffer.length()-6));
							buffer.delete(0, buffer.length());
						}
					}
						// H1
				else if (buffer.toString().endsWith("=====")) {
						if (content.length() < i || content.charAt(i + 1) != '=') {
							if (state.empty()) {
								elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 5), TextBlock.ATTR_NORM));
								//System.out.println("added normal: "+buffer.toString().substring(0, buffer.length()-5));
								buffer.delete(0, buffer.length());
								state.add("=====");
							} else if (state.peek().equals("=====")) {
								state.pop();
								if (state.empty()) {
									elements.add(new TextBlock(
											parseText(buffer.toString().substring(0, buffer.length() - 5))
											, TextBlock.ATTR_H3)
									);
									//System.out.println("added h3: "+buffer.toString().substring(0, buffer.length()-5));
									buffer.delete(0, buffer.length());
								}
							}
						}
					}
						// H2
					else if (buffer.toString().endsWith("====")) {
						if (content.length() < i || content.charAt(i + 1) != '=') {
							if (state.empty()) {
								elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 4), TextBlock.ATTR_NORM));
								//System.out.println("added normal: "+buffer.toString().substring(0, buffer.length()-4));
								buffer.delete(0, buffer.length());
								state.add("====");
							} else if (state.peek().equals("====")) {
								state.pop();
								if (state.empty()) {
									elements.add(new TextBlock(
											parseText(buffer.toString().substring(0, buffer.length() - 4))
											, TextBlock.ATTR_H4)
									);
									//System.out.println("added h4: "+buffer.toString().substring(0, buffer.length()-4));
									buffer.delete(0, buffer.length());
								}
							}
						}
					}

					// H3
					else if (buffer.toString().endsWith("===")) {
						if (content.length() < i || content.charAt(i + 1) != '=') {
							if (state.empty()) {
								elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 3), TextBlock.ATTR_NORM));
								//System.out.println("added normal: "+buffer.toString().substring(0, buffer.length()-3));
								buffer.delete(0, buffer.length());
								state.add("===");
							} else if (state.peek().equals("===")) {
								state.pop();
								if (state.empty()) {
									elements.add(new TextBlock(
											parseText(buffer.toString().substring(0, buffer.length() - 3))
											, TextBlock.ATTR_H5)
									);
									//System.out.println("added h5: "+buffer.toString().substring(0, buffer.length()-3));
									buffer.delete(0, buffer.length());
								}
							}
						}
					}
						// Used to transform plaintext url into LinkContent
					else if (buffer.toString().endsWith("www.")) {
						if (state.empty()) {
							elements.add(new TextBlock(buffer.toString()
								.substring(0, buffer.length() - 4), TextBlock.ATTR_NORM));

							//System.out.println("Added normal in www: "+buffer.toString().substring(0, buffer.length()-4));
							buffer.delete(0, buffer.length());
							state.add("www.");
						}
					}
						// Used to transform plaintext url into LinkContent
					else if (buffer.toString().endsWith("http://")) {
						if (state.empty()) {
							elements.add(new TextBlock(buffer.toString()
								.substring(0, buffer.length() - 7), TextBlock.ATTR_NORM));

							//System.out.println("Added normal in http: "+buffer.toString().substring(0, buffer.length()-7));
							buffer.delete(0, buffer.length());
							state.add("http://");
						}
					}
						// Use to prevent http:// from being parsed as italic
					else if (!buffer.toString().endsWith("http://")) {
						if (buffer.toString().endsWith("//")) {
							if (state.empty()) {
								elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 2), TextBlock.ATTR_NORM));

								//System.out.println("added norma in // "+buffer.toString().substring(0, buffer.length()-2));
								buffer.delete(0, buffer.length());
								state.add("//");
							} else if (state.peek().equals("//")) {
								state.pop();
								if (state.empty()) {
									elements.add(new TextBlock(buffer.toString().substring(0, buffer.length() - 2), TextBlock.ATTR_ITALIC));
									//System.out.println("added italic: "+buffer.toString().substring(0, buffer.length()-2));
									buffer.delete(0, buffer.length());
								}
							}
						}
					}
			}
		}
		//System.out.println("Added normal end:"+buffer);
		elements.add(new TextBlock(buffer.toString(), TextBlock.ATTR_NORM));
		results.addAll(elements);

		return results;
	}

	private LinkContent parseLink(String content) throws Exception {
		//[[http://www.php.net|LINK TEXT]]
		//System.out.println("Parsing link: "+content);
		String url = "";
		int type = 2;
		ArrayList<ContentElement> elements = new ArrayList<ContentElement>();

		content = content.trim();
		content = content.substring(0, content.length());
		if(content.contains("|")){
			url = content.substring(0, content.indexOf("|"));
			if(!url.contains("http://") && !url.isEmpty())
				type = LinkContent.LINK_INTERNAL;
			else
				type = LinkContent.LINK_EXTERNAL;

			elements = parseText(content.substring(content.indexOf("|")+1));
		}
		else {
			url = content;
		}


		//System.out.println("URL: "+url);

		return new LinkContent(url, elements, type);
	}

	private ImageContent parseImage(String content) throws Exception {
		//{http://de3.php.net/images/php.gif?200x50}}
		//System.out.println("Parsing Image: "+content);
		String url = "";
		String alt = "";
		String width = "";
		String height = "";
		BufferedImage img = null;

		content = content.trim();
		content = content.substring(0, content.length());
		if(content.contains("?")) {
			url = content.substring(0, content.indexOf("?"));
			content = content.substring(content.indexOf("?")+1);

			if(content.contains("x")) {
				width = content.substring(0, content.indexOf("x"));
				height = content.substring(content.indexOf("x")+1);
			}
			else
				width = content;
		}
		else if(content.contains("|")){
			url = content.substring(0, content.indexOf("|"));
			content = content.substring(content.indexOf("|"));
			alt = content.substring(content.indexOf("|")+1);
		}
		else {
			url = content;
		}

		//System.out.println("====================== IMAGES ===========================");
		//System.out.println("url: "+url);
		/*if(!url.isEmpty() && !url.contains("http://")) {
			//System.out.println("Internal image");
			//System.out.println("path: "+new File(url).getAbsolutePath());
			// Get image into object
			try {
			    //System.out.println("Saving Image");
				img = ImageIO.read(new File(url));
				//System.out.println("Image saved: "+img.getWidth());
			}
			catch (IOException e) {
				System.out.println(e.getMessage());
				System.out.println("Internal image: " + url);
				throw e;
			}
		}*/

		/*
		//System.out.println("URL: "+url);
		//System.out.println("width: "+width+", height: "+height);
		//System.out.println("ALT: "+alt);
		*/
		return new ImageContent(url, alt, width, height, img);
	}


	String readFile(File file) throws Exception {
		BufferedReader in = new BufferedReader(new FileReader(file));

		String inLine = null;
		StringBuffer output = new StringBuffer();

		while ((inLine = in.readLine()) != null) {
            output.append(inLine);
        }

		return output.toString();
	}

	ArrayList<Doc> mergesort(ArrayList<Doc> docs) {
		List<Doc> sorted = mergesort(docs.subList(0, docs.size()));
		ArrayList<Doc> result = new ArrayList<Doc>();
		for(int i = 0; i < sorted.size(); i++) {
			result.add(sorted.get(i));
		}
		return result;
	}

	List<Doc> mergesort(List<Doc> docs) {
		if(docs.size() > 1) {
			return merge(mergesort(docs.subList(0, docs.size()/2))
					,mergesort(docs.subList((docs.size()/2), docs.size()))
					);
		}
		else {
			return docs;
		}
	}
	List<Doc> merge(List<Doc> left, List<Doc> right) {
		ArrayList<Doc> result = new ArrayList<Doc>();

		int i = 0, j = 0;
		while(i < left.size() && j < right.size()) {
			if(left.get(i).getID().compareToIgnoreCase(right.get(j).getID()) < 0) {
				result.add(left.get(i));
				i++;
			}
			else {
				result.add(right.get(j));
				j++;
			}
		}

		while(i < left.size()) {
			result.add(left.get(i));
			i++;
		}

		while(j < right.size()) {
			result.add(right.get(j));
			j++;
		}

		return result;
	}
}
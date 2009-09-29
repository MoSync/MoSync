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
 * ContentElement.java - Contain Java representation of documentation nodes and entities
 * 
 * Last edited: 22 May 2008
 * 
 */

package com.mobilesorcery.mosync.docbook;

import java.awt.image.BufferedImage;
import java.util.*;

/** Abstract class to represent an document element from Section to Text
 * 
 * @author Aaron
 *
 */
abstract class ContentElement {
	static final int TYPE_SNIPPET = 1;
	static final int TYPE_PARA = 2;
	static final int TYPE_LIST = 3;
	static final int TYPE_SECTION = 4;	
	static final int TYPE_IMAGE = 5;	
	static final int TYPE_LINK = 6;
	static final int TYPE_TEXT = 7;
	static final int TYPE_CODE_SECTION = 8;
	protected final int type;
	ContentElement(int type){
		this.type = type;
	}
	final int getType() { return type; }
}

/** Doc
 * 
 * @author Aaron
 *
 */
 
 class DocFolder {

	public DocFolder(String name) {
		this.name = name;
		this.parent = parent;
		children = new ArrayList<DocFolder>();
		docs = new ArrayList<Doc>();
	}
	
	protected void setParent(DocFolder parent) {
		this.parent = parent;
	}
	
	public void addChild(DocFolder dc) {
		children.add(dc);
		dc.setParent(this);
	}
	
	public void addDoc(Doc doc) {
		doc.setParent(this);
		docs.add(doc);
		
	}
	
	String getName() {
		return name;
	}

	ArrayList<DocFolder> getChildren() {
		return children;
	}

	ArrayList<Doc> getDocs() {
		return docs;
	}
	
	DocFolder getParent() {
		return parent;
	}
	
	private DocFolder parent;
	private String name;
	private ArrayList<DocFolder> children;
	private ArrayList<Doc> docs;
 }
 
 
class Doc {
	private ArrayList<Section> sections;
	private Map<String, String> snippets;
	
	private String author;
	private String title;
	private String id;
	private String[] areas;
	private String[] keywords;
	private DocFolder parent;
		
	Doc(String id, String title, String author, String[] areas, String[] keywords) {
		this.id = id;
		this.title = title;
		this.author = author;
		this.areas = areas;
		this.keywords = keywords;
		
		sections = new ArrayList<Section>();
		snippets = new HashMap<String, String>();
	}

	Map<String, String> getSnippets() {
		return snippets;
	}

	ArrayList<Section> getSections() {
		return sections;
	}
	
	void addSection(Section section) throws Exception{
		for(int i = 0; i < sections.size(); i++) {
			if(sections.get(i).getID() == section.getID()) {
				throw new Exception("Duplicate section id.");
			}
		}
		sections.add(section);
	}
	
	public String getID() {
		return id;
	}
	
	String getAuthor() {
		return author;
	}
	
	public String getTitle() {
		return title;
	}
	
	String[] getAreas() {
		return areas;
	}
	
	String[] getKeywords() {
		return keywords;
	}
	
	DocFolder getParent() {
		return parent;
	}
	
	void setParent(DocFolder parent) {
		this.parent = parent;
	}
}

class CodeSnippet extends ContentElement {
	private String id;
	private Map<String, String> snippets;
	CodeSnippet(Map<String, String> s, String i) {
		super(TYPE_SNIPPET);
		snippets = s;
		id = i;
	}
	String getSource() throws Exception {
		String s = snippets.get(id);
		if (s == null)
			throw new Exception("Unknown snippet used: " + id);
		return s;
	}
}

class CodeSection extends ContentElement {
	private String filename;
	private String source;
	CodeSection(String s, String f) {
		super(TYPE_CODE_SECTION);
		source = s;
		filename = f;
	}

	String getSource() {
		return source;
	}
	String getFilename() {
		return filename;
	}
}

class TextContent extends ContentElement {
	private String text;

	TextContent(String s) {
		super(TYPE_TEXT);
		text = s;
	}
	String getText() {
		return text;
	}
}

class TextBlock extends ContentElement {
	static final int ATTR_EMPH 			= 1;
	static final int ATTR_SYSCALL 		= 2;
	static final int ATTR_BOLD 			= 3;
	static final int ATTR_ITALIC 		= 4;
	static final int ATTR_UNDERLINED 	= 5;
	static final int ATTR_MONOSPACED 	= 6;
	static final int ATTR_NORM 			= 7;
	static final int ATTR_DEL 			= 8;
	static final int ATTR_SUB 			= 9;
	static final int ATTR_NOTE 			= 10;
	static final int ATTR_SUP 			= 11;
	static final int ATTR_QUOTE 		= 12;
	static final int ATTR_H3 			= 13;
	static final int ATTR_H4 			= 14;
	static final int ATTR_H5			= 15;
	static final int ATTR_HR 			= 16;
	
	private int attributes;
	private ArrayList<ContentElement> contents;
	
	TextBlock() {
		super(TYPE_TEXT);
	}
	
	TextBlock(int attributes) {
		this();
		this.contents = new ArrayList<ContentElement>();
		this.attributes = attributes;
	}
	
	TextBlock(String text, int attributes) {
		this(new TextContent(text), attributes);
	}
	
	TextBlock(ContentElement element, int attributes) {
		this(attributes);
		this.contents.add(element);
	}
	
	TextBlock(ArrayList<ContentElement> elements, int attributes) {
		this(attributes);
		this.contents.addAll(elements);
	}
	
	int getAttributes() { return attributes; }
	
	ArrayList<ContentElement> getContents() { return contents; }
	
	void addContent(ContentElement element) {
		this.contents.add(element);
	}
	
}

class LineBreak extends ContentElement {
	LineBreak() {
		super(TYPE_TEXT);
	}
}

class ImageContent extends ContentElement{
	private String url;
	private String alt;
	private String width;
	private String height;
	private BufferedImage bImage;
	
	ImageContent(String url, String alt, String width, String height) {
		super(TYPE_IMAGE);
		this.url = url;
		this.alt = alt;
		this.width = width;
		this.height = height;
		this.bImage = null;
	}
	
	ImageContent(String url, String alt, String width, String height, BufferedImage bImage) {
		this(url, alt, width, height);
		this.bImage = bImage;
	}
	
	String getURL() {
		return url;
	}
	
	String getAlt() {
		return alt;
	}
	
	String getHeight() {
		return height;
	}
	
	String getWidth() {
		return width;
	}
	
	BufferedImage getBImage() {
		return bImage;
	}
	
}

class LinkContent extends ContentElement{
	private String url;
	private ArrayList<ContentElement> elements;
	private int type;

	static final int LINK_EXTERNAL 			= 1;
	static final int LINK_INTERNAL 			= 2;
	
	LinkContent() {
		super(TYPE_LINK);
		this.elements = new ArrayList<ContentElement>();
	}
	
	LinkContent(String url, ContentElement element, int type) {
		this();
		this.url = url;
		this.elements.add(element);
		this.type = type;
	}
	
	LinkContent(String url, ArrayList<ContentElement> elements, int type) {
		this();
		this.url = url;
		this.elements.addAll(elements);
		this.type = type;
	}
	
	String getURL() {
		return url;
	}
	
	ArrayList<ContentElement> getElements() {
		return elements;
	}
}

class Paragraph extends ContentElement{
	private ArrayList<ContentElement> blocks;
	
	Paragraph() {
		super(TYPE_PARA);
		blocks = new ArrayList<ContentElement>();
	}
	
	Paragraph(ArrayList<ContentElement> elements) {
		this();
		this.blocks.addAll(elements);
	}
	void addContentElement(ContentElement cElement) {
		blocks.add(cElement);
	}
	
	void addContentElements(ArrayList<ContentElement> elements) {
		this.blocks.addAll(elements);
	}
	
	ArrayList<ContentElement> getBlocks() {
		return blocks;
	}
}

class ListContent extends ContentElement{
	private ArrayList<ContentElement> elements;
	private boolean ordered;
	
	ListContent(boolean ordered) {
		super(TYPE_LIST);
		elements = new ArrayList<ContentElement>();
		this.ordered = ordered;
	}
	
	void addContentElement(ContentElement cElement) { 
		this.elements.add(cElement);
	}
	
	void addContentElements(ArrayList<ContentElement> cElements) { 
		this.elements.addAll(cElements);
	}
	
	void addBlock(String text, int attributes) { 
		this.elements.add(new TextBlock(text, attributes));
	}
	
	void addList(ListContent list) { 
		this.elements.add(list);
	}
	
	ArrayList<ContentElement> getChilds() {
		return this.elements;
	}
	
	boolean getOrdered() {
		return ordered;
	}
}


class Section extends ContentElement {
	
	private ArrayList<ContentElement> elements;
	private String id;
	private String title;

	Section() {
		super(TYPE_SECTION);
		elements = new ArrayList<ContentElement>();
	}
	
	Section(String id, String title) {
		this();
		this.title = title;
		this.id = id;
	}
	
	void addContentElement(ContentElement cElement) {
		elements.add(cElement);
	}
	
	void addContentElement(ArrayList<ContentElement> cElements) {
		for(int i = 0; i < cElements.size(); i++) {
			this.addContentElement(cElements.get(i));
		}
	}
	
	/*String generateID(String title) {
		String id = title.toLowerCase().replace(' ', '_');
		return id;
	}*/
	
	ArrayList<ContentElement> getElements() {
		return elements;
	}
	
	void setID(String id) {
		this.id = id;
	}
	
	String getID() {
		return this.id;
	}
	
	String getTitle() {
		return title;
	}
}

class Area {
	static final int AREA_GRAPHIC 		= 1;
	static final int AREA_NETWORKING	= 2;
}


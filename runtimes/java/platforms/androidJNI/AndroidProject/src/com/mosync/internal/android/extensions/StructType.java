package com.mosync.internal.android.extensions;

import java.util.ArrayList;
import java.util.Map;

public class StructType extends TypeDescriptor {

	private String type;
	private ArrayList<String> memberNames = new ArrayList<String>();
	private ArrayList<String> memberTypeNames = new ArrayList<String>();
	private Map<String, TypeDescriptor> typedefs;
	private String className;
	private Class clazz;
	private IStruct prototype;

	public StructType(ExtensionModule module, String type, String className) throws Exception {
		this.type = type;
		this.className = className;
		//this.clazz = Class.forName(className);
		//this.prototype = (IStruct) clazz.newInstance();
	}

	public void addMember(String name, String typename) {
		memberNames.add(name);
		memberTypeNames.add(typename);
	}

	public String getTypeName() {
		return type;
	}

	public Class getNativeClass() {
		return clazz;
	}

	public Object convert(int[] args, int offset) {
		return prototype.convert(args, offset);
	}

	public String toString() {
		StringBuffer buf = new StringBuffer();
		buf.append("\n{\n");
		for (int i = 0; i < memberNames.size(); i++) {
			buf.append(memberTypeNames.get(i));
			buf.append(" ");
			buf.append(memberNames);
			buf.append(";\n");
		}
		buf.append("}\n");
		return buf.toString();
	}

}
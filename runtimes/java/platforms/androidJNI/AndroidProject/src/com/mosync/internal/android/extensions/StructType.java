package com.mosync.internal.android.extensions;

import java.util.ArrayList;
import java.util.Map;

import com.mosync.api.Struct;

public class StructType extends TypeDescriptor {

	private String type;
	private ArrayList<String> memberNames = new ArrayList<String>();
	private ArrayList<String> memberTypeNames = new ArrayList<String>();
	private ArrayList<Integer> memberPtrDepths = new ArrayList<Integer>();
	private Map<String, TypeDescriptor> typedefs;
	private String className;
	private Class clazz;
	private Struct prototype;
	private ArrayList<TypeDescriptor> memberTypes;
	private ExtensionModule module;
	private StructType outStruct;

	public StructType(ExtensionModule module, String type, String className) throws Exception {
		this.module = module;
		this.type = type;
		this.className = className;
		this.clazz = Class.forName(className);
		this.prototype = (Struct) clazz.newInstance();
		this.outStruct = new StructType(this, true);
	}

	private StructType(StructType original, boolean out) {
		this.module = original.module;
		this.type = original.type;
		this.className = original.className;
		this.clazz = original.clazz;
		this.prototype = original.prototype;
		setOut(out);
	}

	public StructType getOutType() {
		return outStruct;
	}

	public void addMember(String name, String typename, int ptrDepth) {
		memberNames.add(name);
		memberTypeNames.add(typename);
		memberPtrDepths.add(ptrDepth);
	}

	public String getTypeName() {
		return type;
	}

	public Class getNativeClass() {
		return clazz;
	}

	public Object unmarshal(byte[] data, int offset) {
		Struct result = (Struct) prototype.unmarshal(data, offset);
		return result;
	}

	@Override
	public void marshal(Object o, byte[] data, int offset) {
		prototype.marshal(o, data, offset);
	}

	public int size() {
		return prototype.size();
	}

	public String toString() {
		StringBuffer buf = new StringBuffer();
		buf.append("\n{\n");
		for (int i = 0; i < memberNames.size(); i++) {
			buf.append(memberTypeNames.get(i));
			for (int j = 0; j < memberPtrDepths.get(0); j++) {
				buf.append("*");
			}
			buf.append(" ");
			buf.append(memberNames.get(i));
			buf.append(";\n");
		}
		buf.append("}\n");
		return buf.toString();
	}

}
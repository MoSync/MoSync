package com.mosync.internal.android.extensions;

import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;

import com.mosync.internal.android.MoSyncThread;

public class FunctionInvocation {
	private TypeDescriptor[] tds;
	private TypeDescriptor rd;
	private ExtensionModule module;
	private String name;
	private Method resolvedMethod;
	private int id;

	public FunctionInvocation(int id, ExtensionModule module, String name,
			TypeDescriptor[] tds, TypeDescriptor rd) throws Exception {
		this.id = id;
		this.module = module;
		this.name = name;
		this.tds = tds;
		this.rd = rd;

		Class[] parameterTypes = new Class[tds.length];
		for (int i = 0; i < parameterTypes.length; i++) {
			TypeDescriptor resolvedParameterType = tds[i];
			parameterTypes[i] = resolvedParameterType.getNativeClass();
		}
		resolvedMethod = module.getModule().getClass().getMethod(name,
				parameterTypes);
	}

	public int invoke(int ptr) throws Throwable {
		Object[] convertedArgs = new Object[resolvedMethod
				.getParameterTypes().length];
		int[] argAndRetPtrs = new int[convertedArgs.length + 1];
		MoSyncThread mThread = MoSyncThread.getInstance();
		// Get the pointers to the arguments to use.
		ByteBuffer argBuffer = mThread.getMemorySlice(ptr, 4 * argAndRetPtrs.length);
		IntBuffer intBuffer = argBuffer.order(null).asIntBuffer();
		intBuffer.position(0);
		for (int i = 0; i < argAndRetPtrs.length; i++) {
			argAndRetPtrs[i] = intBuffer.get();
		}

		for (int i = 0; i < argAndRetPtrs.length - 1; i++) {
			// Special case: non-pointer structs have been sent as pointers,
			// but if it already is a pointer then we need not add an extra
			// layer. (This comes all the way from the IDL compiler.)
			if (tds[i] instanceof StructType) {
				argAndRetPtrs[i] = IntType.readIntFromMem(argAndRetPtrs[i]);
			}
		}
		for (int i = 0; i < argAndRetPtrs.length - 1; i++) {
			convertedArgs[i] = tds[i].readFromMemory(argAndRetPtrs[i]);
		}

		Object result = resolvedMethod.invoke(module.getModule(), convertedArgs);

		for (int i = 0; i < tds.length; i++) {
			if (tds[i].isOut()) {
				tds[i].writeToMemory(argAndRetPtrs[i], convertedArgs[i]);
			}
		}

		if (rd != null && VoidType.getInstance() != rd) {
			rd.writeToMemory(argAndRetPtrs[argAndRetPtrs.length - 1], result);
		}
		return 0;
	}

	public String getName() {
		return name;
	}

	public int getId() {
		return id;
	}

}
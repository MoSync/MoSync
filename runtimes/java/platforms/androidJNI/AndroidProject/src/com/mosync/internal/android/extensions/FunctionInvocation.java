package com.mosync.internal.android.extensions;

import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;

import com.mosync.internal.android.MoSyncThread;

public class FunctionInvocation {
	private TypeDescriptor[] tds;
	private ExtensionModule module;
	private String name;
	private Method resolvedMethod;
	private int id;

	public FunctionInvocation(int id, ExtensionModule module, String name,
			TypeDescriptor[] tds) throws Exception {
		this.id = id;
		this.module = module;
		this.name = name;
		this.tds = tds;

		Class[] parameterTypes = new Class[tds.length];
		for (int i = 0; i < parameterTypes.length; i++) {
			TypeDescriptor resolvedParameterType = tds[i].resolve();
			parameterTypes[i] = resolvedParameterType.getNativeClass();
		}
		resolvedMethod = module.getModule().getClass().getMethod(name,
				parameterTypes);
	}

	public int invoke(int ptr) throws Throwable {
		Object[] convertedArgs = new Object[resolvedMethod
				.getParameterTypes().length];
		int[] args = new int[convertedArgs.length];
		MoSyncThread mThread = MoSyncThread.getInstance();
		ByteBuffer argBuffer = mThread.getMemorySlice(ptr, 4 * args.length);
		IntBuffer intBuffer = argBuffer.order(null).asIntBuffer();
		intBuffer.position(0);
		for (int i = 0; i < args.length; i++) {
			args[i] = intBuffer.get();
		}

		for (int i = 0; i < args.length;) {
			convertedArgs[i] = tds[i].convert(args, i);
			i += tds[i].size();
		}

		Object result = resolvedMethod.invoke(module.getModule(), convertedArgs);
		if (result instanceof Integer) {
			return (Integer) result;
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
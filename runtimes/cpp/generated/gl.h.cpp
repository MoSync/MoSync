static MAHandle glGetStringHandle(unsigned int name) {
	MAHandle placeholder = maCreatePlaceholder();
	const GLubyte* ret = glGetString(name);
	return SYSCALL_THIS->resources.add_RT_BINARY(placeholder, new MemStreamC(ret, strlen((char*)ret)+1));
}


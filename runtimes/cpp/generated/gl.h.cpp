static MAHandle glGetStringHandle(unsigned int name) {
	MAHandle placeholder = maCreatePlaceholder();
	const GLubyte* ret = glGetString(name);
	SYSCALL_THIS->resources.add_RT_BINARY(placeholder, new MemStreamC(ret, strlen((char*)ret)+1));
    return placeholder;
}


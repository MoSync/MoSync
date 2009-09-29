#ifndef __MOSYNC_PAN__
#define __MOSYNC_PAN__

/** MoSync application panic codes */
enum TMoSyncPanics {
	EMoSyncUnloggedPanic = 0,
	EMoSyncLoggedPanic = 10,
	EMoSyncUi
	//ECreateTrapCleanup
	//EMainSchedulerError
	// add further panics here
};

void Panic(TMoSyncPanics aReason)
#ifdef __GCCE__
__attribute((noreturn))
#endif
;

inline void Panic(TMoSyncPanics aReason) {
	_LIT(applicationName, "MoSync");
	User::Panic(applicationName, aReason);
	while(1) {}
}

#endif // __MOSYNC_PAN__

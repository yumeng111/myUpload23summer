/*****************************************************************************\
* $Id: VMELock.cc,v 1.4 2010/04/26 09:55:13 paste Exp $
\*****************************************************************************/
#include "emu/fed/VMELock.h"
#include <sstream>
#include <sys/file.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> // for close


emu::fed::VMELock::VMELock(const std::string &fileName)
throw (emu::exception::SoftwareException):
lockfile_(fileName),
fd_(-1),
nFileLock_(0)
{
	// Try to open the file (see http://pubs.opengroup.org/onlinepubs/009695399/functions/open.html)
	// making sure other people can use it.
        mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	if ((fd_ = open(lockfile_.c_str(), O_CREAT, mode)) < 0) {
		std::ostringstream error;
		error << "Error opening lock file " << lockfile_ << ":  permissions problem?";
		XCEPT_RAISE(emu::exception::SoftwareException, error.str());
	}

	// // Make sure other people can use this file
	// if (fchmod(fd_, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)) {
	// 	std::ostringstream error;
	// 	error << "Error changing permissions of lock file " << lockfile_;
	// 	XCEPT_RAISE(emu::exception::SoftwareException, error.str());
	// }

	// Initialize the mutex
	pthread_mutexattr_t mutexAttr;
	pthread_mutexattr_init(&mutexAttr);
	int err = pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE_NP);
	if (err) {
		std::ostringstream error;
		error << "Unable to set mutex into recursive mode: " << err;
		XCEPT_RAISE(emu::exception::SoftwareException, error.str());
	}
	err = pthread_mutex_init(&mutex_, &mutexAttr);
	if (err) {
		std::ostringstream error;
		error << "Unable to initialize mutex: " << err;
		XCEPT_RAISE(emu::exception::SoftwareException, error.str());
	}
}



emu::fed::VMELock::~VMELock()
{
	// Make sure everything is unlocked
	if (nFileLock_) {
		flock(fd_, LOCK_UN);
	}
	if (fd_ >= 0) close(fd_);
}



void emu::fed::VMELock::lock()
throw (emu::exception::SoftwareException)
{
	//std::cout << "lock" << std::endl;
	// First do the non-expensive locking of the process
	int err = pthread_mutex_lock(&mutex_);
	if (err) {
		std::ostringstream error;
		error << "Error locking mutex: " << err;
		XCEPT_RAISE(emu::exception::SoftwareException, error.str());
	}

	// Now try to lock the file
	err = flock(fd_, LOCK_EX);
	if (err) {
		std::ostringstream error;
		error << "Error locking file " << lockfile_ << ": " << err;
		XCEPT_RAISE(emu::exception::SoftwareException, error.str());
	}
	nFileLock_++;
}



void emu::fed::VMELock::unlock()
throw (emu::exception::SoftwareException)
{
	//std::cout << "unlock" << std::endl;
	// First, try to unlock the file
	if (nFileLock_ == 1) {
		int err = flock(fd_, LOCK_UN);
		if (err) {
			std::ostringstream error;
			error << "Error unlocking file " << lockfile_ << ": " << err;
			XCEPT_RAISE(emu::exception::SoftwareException, error.str());
		}
		nFileLock_ = 0;
	} else if (nFileLock_ > 0) {
		nFileLock_--;
	}

	// Last, do the non-expensive unlocking of the process
	int err = pthread_mutex_unlock(&mutex_);
	if (err) {
		std::ostringstream error;
		error << "Error unlocking mutex: " << err;
		XCEPT_RAISE(emu::exception::SoftwareException, error.str());
	}
}

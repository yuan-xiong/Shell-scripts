#include <iostream>
#include <fcntl.h>	// for open
#include <unistd.h>	// for write
#include <dirent.h>	// for dirent
#include <stdio.h>	// for snprintf
#include <string.h>	// for strcmp

using namespace std;


std::string get_file_name (const int fd)
{
	//本文来自 cywosp 的CSDN 博客 ，全文地址请点击:
	//https://blog.csdn.net/cywosp/article/details/9147467?utm_source=copy 
	if (0 >= fd) {
	    return std::string ();
	}
	
	char buf[1024] = {'\0'};
	char file_path[PATH_MAX] = {'0'}; // PATH_MAX in limits.h
	snprintf(buf, sizeof (buf), "/proc/self/fd/%d", fd);
	if (readlink(buf, file_path, sizeof(file_path) - 1) != -1) {
	    return std::string (file_path);
	}
	
	return std::string ();
}

void mycompare() {
	char *cstring1, *cstring2;
	string string1, string2;

	cstring1 = cstring2 = (char*)"test";
	cout << "cstring1\t" << cstring1 << endl;
	cout << "cstring2\t" << cstring2 << endl << endl;
	string1 = string2 = "test";
	cout << "string1\t\t" << string1 << endl;
	cout << "string2\t\t" << string2 << endl << endl;
	
	bool cc = (cstring1 == cstring2);
	cout << "cstring1 == cstring2\t" << cc << endl;
	bool cs = (string1 == string2);
	cout << "string1 == string2\t" << cs << endl << endl;

	cc = (cstring1 == string1);
	cout << "cstring1 == string1\t" << cc << endl;
	cs = (cstring2 == string2.c_str());
	cout << "cstring2 == string2.c_str()\t" << cs << endl;
	cs = (strcmp(cstring2, string2.c_str()) == 0);
	cout << "strcmp(cstring2,string2.c_str())\t" << cs << endl << endl;
}


void write_file_uevent(const char* file) {
	cout << "write_file_uevent: " << file << endl;	

	int fd = 0;
	if ((fd = open(file, O_RDWR)) < 0) {
		cout << "write_uevent: open failed, no such file" << endl;	
		return;
	}

	if ((write(fd, "add\n", 4)) < 0) {
		cout << "write_uevent: write failed" << endl;	
	}
	close(fd);
}

void do_coldboot(DIR *d) {
	struct dirent *de;
	int dfd, fd;

	dfd = dirfd(d);
	//cout << "\n ---> xy do_coldboot\t" << get_file_name(dfd) << endl;	

	// 1. file
	fd = openat(dfd, "uevent", O_WRONLY);
	if (fd >= 0) {
		//cout << "openat ok" << endl;	
		if ((write(fd, "add\n", 4)) < 0) {
			cout << "write_uevent: write failed" << endl;	
		}
		cout << "do_coldboot: write add\tfd " << fd << "\t" << get_file_name(fd) << endl;	
		close(fd);
	} else {
		//cout << "do_coldboot no uevent at\t" << get_file_name(dfd) << endl;	
	}

	// 2. dir
	while ((de = readdir(d))) {
		DIR *d2;

		if (de->d_type != DT_DIR || de->d_name[0] == '.')
			continue;

		//cout << "dir:\t" << get_file_name(dfd) << "//" << de->d_name << endl;
		//if (de->d_name == "virtual") {
		//	continue;
		//}

		fd = openat(dfd, de->d_name, O_RDONLY | O_DIRECTORY);
		if (fd < 0)
			continue;
		d2 = fdopendir(fd);
		if (d2 == 0) {
			close(fd);
		} else {
			do_coldboot(d2);
			closedir(d2);
		}
	}
}

void coldboot(const char* path) {
	cout << " ---> xy coldboot:\t" << path << endl;	
	DIR *d = opendir(path);
	if (d) {
		do_coldboot(d);
		closedir(d);
	} else {
		cout << "Not a dirent" << endl;
	}
}


int main() {
	//mycompare();
			//write_file_uevent("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/sda1/uevent");

	coldboot("/sys/block");
	coldboot("/sys/class");
	//coldboot("/sys/devices");
		coldboot("/sys/devices/breakpoint");
		coldboot("/sys/devices/cpu");
		coldboot("/sys/devices/cstate_core");
		coldboot("/sys/devices/cstate_pkg");
		coldboot("/sys/devices/intel_bts");
		coldboot("/sys/devices/intel_pt");
		coldboot("/sys/devices/isa");
		coldboot("/sys/devices/LNXSYSTM:00");
		coldboot("/sys/devices/msr");
		//coldboot("/sys/devices/pci0000:00");
			coldboot("/sys/devices/pci0000:00/0000:00:00.0");
			coldboot("/sys/devices/pci0000:00/0000:00:02.0");
			coldboot("/sys/devices/pci0000:00/0000:00:03.0");
			coldboot("/sys/devices/pci0000:00/0000:00:14.0");
			coldboot("/sys/devices/pci0000:00/0000:00:16.0");
			coldboot("/sys/devices/pci0000:00/0000:00:19.0");
			coldboot("/sys/devices/pci0000:00/0000:00:1a.0");
			coldboot("/sys/devices/pci0000:00/0000:00:1b.0");
			coldboot("/sys/devices/pci0000:00/0000:00:1c.0");
			coldboot("/sys/devices/pci0000:00/0000:00:1c.1");
			coldboot("/sys/devices/pci0000:00/0000:00:1d.0");
			coldboot("/sys/devices/pci0000:00/0000:00:1f.0");
			//coldboot("/sys/devices/pci0000:00/0000:00:1f.2");
				coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata1");		
				//coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2");
					coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/ata_port");
					//coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1");
						coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/power");
						coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/scsi_host");
						coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/subsystem");
						//coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0");	//unshare + snap
							//coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0");
								//coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block");
									//coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda");
										coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/bdi");
										//coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/device");
										coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/holders");
										coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/integrity");
										coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/power");
										coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/queue");
										//coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/sda1"); // snap(unshare)
											coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/sda1/holders");
											coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/sda1/power");
											coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/sda1/subsystem");
											coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/sda1/trace");
											//write_file_uevent("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/sda1/uevent");
										//coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/sda2"); // snap(unshare)
										//coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/sda3"); // snap(unshare)
										//coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/sda4"); // snap(unshare)
										//coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/sda5"); // snap(unshare)
										coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/slaves");
										coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/subsystem");
										coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/trace");
										//write_file_uevent("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/block/sda/uevent"); //snap(unshare)
								coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/bsg");
								coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/driver");
								coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/generic");
								coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/power");
								coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/scsi_device");
								coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/scsi_disk");
								coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/scsi_generic");
								coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/subsystem");
								write_file_uevent("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/1:0:0:0/uevent");
							coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/power");
							coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/subsystem");
							write_file_uevent("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/target1:0:0/uevent");
						write_file_uevent("/sys/devices/pci0000:00/0000:00:1f.2/ata2/host1/uevent");
					coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/link2");
					coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata2/power");
					//write_file_uevent("/sys/devices/pci0000:00/0000:00:1f.2/ata2/uevent");
				//coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata3");	//snap
				coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata4");
				coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata5");
				coldboot("/sys/devices/pci0000:00/0000:00:1f.2/ata6");
				write_file_uevent("/sys/devices/pci0000:00/0000:00:1f.2/uevent");
			coldboot("/sys/devices/pci0000:00/0000:00:1f.3");
			coldboot("/sys/devices/pci0000:00/firmware_node");
			coldboot("/sys/devices/pci0000:00/pci_bus");
			coldboot("/sys/devices/pci0000:00/power");
			write_file_uevent("/sys/devices/pci0000:00/uevent");
		coldboot("/sys/devices/platform");
		coldboot("/sys/devices/pnp0");
		coldboot("/sys/devices/power");
		coldboot("/sys/devices/software");
		coldboot("/sys/devices/system");
		coldboot("/sys/devices/tracepoint");
		coldboot("/sys/devices/uncore_arb");
		coldboot("/sys/devices/uncore_cbox_0");
		coldboot("/sys/devices/uncore_cbox_1");
		coldboot("/sys/devices/uncore_cbox_2");
		coldboot("/sys/devices/uncore_cbox_3");
		coldboot("/sys/devices/uncore_imc");
		//coldboot("/sys/devices/virtual");
			coldboot("/sys/devices/virtual/bdi");
			//coldboot("/sys/devices/virtual/block");
				//coldboot("/sys/devices/virtual/block");
			coldboot("/sys/devices/virtual/dmi");
			coldboot("/sys/devices/virtual/graphics");
			coldboot("/sys/devices/virtual/hwmon");
			coldboot("/sys/devices/virtual/input");
			coldboot("/sys/devices/virtual/mem");
			coldboot("/sys/devices/virtual/misc");
			coldboot("/sys/devices/virtual/net");
			coldboot("/sys/devices/virtual/powercap");
			coldboot("/sys/devices/virtual/ppp");
			coldboot("/sys/devices/virtual/sound");
			coldboot("/sys/devices/virtual/thermal");
			coldboot("/sys/devices/virtual/tty");
			coldboot("/sys/devices/virtual/vc");
			coldboot("/sys/devices/virtual/vtconsole");
			coldboot("/sys/devices/virtual/workqueue");

}


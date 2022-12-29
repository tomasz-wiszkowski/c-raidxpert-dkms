## AMD RaidXpert drivers "updated" for linux 6.x

There are just a few subtle modifications here, but all that is required for the
driver to work should be available.

This is just a temporary solution, hopefully AMD will release updates soon.
It's not very likely that these drivers will be included as part of the kernel
in their current form any time soon, because of the:

### Licensing fun

There are no clear licenses anywhere, other than an explicit mention that there
are two distinct licenses, one governing the rights to the blob, and other - to
the source files.

Part of the license mentions that:

```
(iii) this source code is the confidential information of DHS and may not be
disclosed to any third party;
```

whatever this means. The sources are openly accessible from AMDs download
page; in fact - this is how these have been acquired.

Lastly, there are no mentions that the open-source part cannot be modified, other
than:

```
(iv) you may not make any modification or take any action that would cause this
software (...) to fall under any GPL license or any other open source license.
```

No such modifications have been made, warranting that the source continues to
remain licensed the same way it was when it was released by AMD.

## Building

Follow regular linux development process, or go through the official building
details included by AMD (I've left them here).

Once you get everything installed, just call `make -C src`.
To build the debian package, execute:

```sh
dpkg-buildpackage -b -uc -us
# produces rcraid-dkms-*.deb in the parent folder
```

## Installing

To make this work, you have to either

### System boot.

Simply download (see downloads page) and install the package:

```sh
sudo dpkg -i rcraid-dkms-*.deb
```

NOTE: While this seems to work, it is highly experimental.

Be prepared that this may render your system non-bootable.
Have a way to revert. Normally this means booting unmodified kernel, or perhaps
the previously installed kernel.

### On demand

Requires you to blacklist just the `ahci` module and manually unbind NVMe
drives that are part of your RAID setup.

To do the latter - this script will match PCI devices and their `/dev/nvme*`
nodes:

```sh
cd /sys/bus/pci/drivers/nvme
for f in 0000:*; do echo $f is $(ls $f/nvme); done
```

e.g.:

```
0000:4c:00.0 is nvme3
0000:21:00.0 is nvme0
0000:43:00.0 is nvme1
0000:44:00.0 is nvme2
```

Next, for each of these nodes that you know that are part of the RAIDXpert
setup, unbind them:

```sh
cd /sys/bus/pci/drivers/nvme
echo 0000:44:00.0 | sudo tee unbind
echo 0000:4c:00.0 | sudo tee unbind
```

Note: `/dev/nvme2*` and `/dev/nvme3*` will become instantly unavailable.

Finally, load the module:

```sh
sudo insmod rcraid.ko
```

A simple script can go a long way here.

## **Known Issues** (no, seriously, please read)
* **`cryptswap`** will likely fail to setup on boot. Consider 
  disabling the `cryptswap` entry in your `/etc/fstab` file ahead of rebooting 
  your system (especially if you installed the `rcraid-dkms.deb` package).
* **`TRIM`** does not appear to be supported. When formatting new partitions,
   use `nodiscard`, e.g.
  ```sh
  sudo mkfs.ext4 /dev/sdX1 -E nodiscard
  ```
  Otherwise mkfs will fail to complete and, well, your system will hang.


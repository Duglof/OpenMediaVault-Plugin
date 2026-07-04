# OpenMediaVault Plugin

This project should enable you to create your own plugin for the OpenMediaVault V8 NAS.
I created a plugin to synchronize my Google Drive with my NAS, display the synchronization status on the dashboard, and view synchronization logs to understand what is happening in the event of an error.
The plugin is created in the new format using YAML and PHP files.

## Références
### Hardware
- NAS Tandberd D400 (internal SSD 2G too small for OpenMediaVault full options)
- 2GB DDR3 RAM upgraded to 6GB 1333 V1.5 204 pins
- Added an 80GB 2.5" drive, mounted in a bracket in the PCI slot and connected to the fifth available SATA port.
- Two 3TB drives set up in a mirror configuration.
- Two available SATA slots for two additional drives

- For install and backup restore OpenMediaVault 80G system drive : 
  - 4GB USB flash drive with Ventoy 1.0.99 + OpenMediaVault V8 ISO and Clonezilla ISO
  - Install Ventoy on the USB then copy iso file on it

### Software
- OpenMediaVault V8 (Test done on V8.4.0-3)
- Rclone v1.74.3 on OpenMediaVault

### Administration
- Linux Mint 22 Cinnamon
- Putty for remote connection on OpenMediaVault NAS (terminal console) 
- Required knowledge : vi editor : Very few orders
| Command | Description |
| vi <filename> | Open or edit a file.
| i       |  Switch to Insert mode |
| Escape  | Switch to Command mode |
| yy      | Yank (copy) a line of text. |
| p       | Paste a line of yanked text below the current line. |
| dd      | dd — Delete an entire line.
| o       | Open a new line under the current line.|
| :w      | Save and continue editing |
| :q!     | Quit vi and do not save changes |
| :x      | Save and quit |

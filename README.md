# OpenMediaVault Plugin

This project should enable you to create your own plugin for the OpenMediaVault V8 NAS.
I created a plugin to synchronize my Google Drive with my NAS, display the synchronization status on the dashboard, and view synchronization logs to understand what is happening in the event of an error.
The plugin is created in the new format using YAML and PHP files.

## Références
### Plugin Development Documentation V8.x
- https://docs.openmediavault.org/en/8.x/development/plugins.html
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
- Required knowledge
  - debian system command

| Command | Description |
|---------------|-----------------------------------------------|
| cp | Copy file |
| rm | Remove file |
| cat | reads the content of a file and print it|
  - vi editor : Very few orders
  
| Command | Description |
|---------------|-----------------------------------------------|
| vi filename | Open or edit a file. |
| i       |  Switch to Insert mode |
| Escape  | Switch to Command mode |
| yy      | Yank (copy) a line of text. |
| p       | Paste a line of yanked text below the current line. |
| dd      | dd — Delete an entire line.
| o       | Open a new line under the current line.|
| :w      | Save and continue editing |
| :q!     | Quit vi and do not save changes |
| :x      | Save and quit |

# Manual file installation
## Plugin name format
The plugin will have two names:

| Noms | Format | Example | Description |
|------------|--------------------|------------------------|-------------------|
| Long name  | omv-name-with-dash | omv-gdrive-john-bisync | Name with dash    |
| Short name | namewithoutdash    | gdrivejohnbisync       | Name without dash |

## Plugin files
To add this plugin that displays its status on the dashboard and visualizes logs, you must create the following files on the system disk:

| Full path name | Description |
|---------|-----|
| /usr/sbin/omv-gdrive-john-bisync-wrapper | wrapper executed every hour |
| /usr/share/openmediavault/engined/rpc/gdrivejohnbisync.inc | remote procedure call |
| /usr/share/openmediavault/workbench/dashboard.d/omv-gdrive-john-bisync-dashboard.yaml | dashboard widgets |
| /usr/share/openmediavault/workbench/component.d/omv-services-gdrivejohnbisync-status-page.yaml | component |
| /usr/share/openmediavault/workbench/navigation.d/services.gdrivejohnbisync.yaml | navigation |
| /usr/share/openmediavault/workbench/route.d/services.gdrivejohnbisync.yaml | route |

- It appears mandatory to follow the file naming format for the plugin to be valid.
- It seems that, in order to appear in the "services" group, certain names must begin with "services."
- Maintain indentation by using spaces.
- The files must be created in this order for the tests to work.
- It seems that creating the files up to the dashboard level is not enough to make it appear; all the files need to be created.
- Preserve the case of filenames and their contents (uppercase, lowercase).

### /usr/sbin/omv-gdrive-john-bisync-wrapper
The wrapper is executed regularly to perform synchronization and creates two result files: `status.json` and `omv-bisync-gdrive-john.log`.

- Create /usr/sbin/omv-gdrive-john-bisync-wrapper with the content displayed by clicking the link below.

[omv-gdrive-john-bisync-wrapper](examples/omv-gdrive-john-bisync/usr/sbin/omv-gdrive-john-bisync-wrapper)

- Grant execute permission to the wrapper using the following command:
  - chmod +x /usr/sbin/omv-gdrive-john-bisync-wrapper
- When the wrapper is executed, the following two files are created:
  - Status file : /var/lib/openmediavault/gdrive-john-bisync/status.json
  - Log file : /var/log/omv-bisync-gdrive-john.log
- It appears mandatory for the fields in the status.json file to meet the following conditions:
  - No empty field
  - No field with the value unknown 
- Manual wrapper test
  - Manually execute the wrapper with the following command:
    - /usr/sbin/omv-gdrive-john-bisync-wrapper
- Checking status.json with the following command:
  - cat /var/lib/openmediavault/gdrive-john-bisync/status.json
  - Expected result:
```
   {
  "status": "error",
  "lastRun": "2026-07-05 18:08:45",
  "message": "Erreur bisync (code 1)"
   }
```
- Checking log file with the following command:
  - cat /var/log/omv-bisync-gdrive-john.log
  - Expected result:
```
2026/07/05 18:08:45 CRITICAL: Failed to create file system for "rclone_omv_gdrive_john:": didn't find section in config file ("rclone_omv_gdrive_john")
```
- The expected result is correct because the latest version of rclone was installed using the following commands:
  - sudo apt install curl -y
  - curl https://rclone.org/install.sh | sudo bash
- However, no configuration has been performed. **The goal here is to create and test the plugin, not to configure rclone**.

### /usr/share/openmediavault/engined/rpc/gdrivejohnbisync.inc
- If you want to adapt the RPC, you must observe the following rules:
  - The class name must be different for each plugin.
  - It appears to be customary to capitalize the beginning of each word.
  - $file = '/var/lib/openmediavault/gdrive-john-bisync/status.json'; must point to your status.json file created by the wrapper
  - $file = '/var/log/omv-bisync-gdrive-john.log'; must point to your status.json file created by the wrapper 
- Create /usr/share/openmediavault/engined/rpc/gdrivejohnbisync.inc with the content displayed by clicking the link below.

[gdrivejohnbisync.inc](examples/omv-gdrive-john-bisync/usr/share/openmediavault/engined/rpc/gdrivejohnbisync.inc)
- After creating the file, you must execute the following command for the RPC to be taken into account:
  - systemctl restart openmediavault-engined
- Test the RPC by executing the following commands:
- omv-rpc gdrivejohnbisync getStatus
```
[{"status":"error","lastRun":"2026-07-05 18:08:45","message":"Erreur bisync (code 1)"}]
```
- omv-rpc gdrivejohnbisync getLog
```
[{"line":"2026\/07\/05 18:08:45 CRITICAL: Failed to create file system for \"rclone_omv_gdrive_john:\": didn't find section in config file (\"rclone_omv_gdrive_john\")"}]
```
- Result in case of error:
```
{"response":null,"error":{"code":0,"message":"RPC service 'gdrivejohnbisync' not found.","trace":"OMV\\HttpErrorException: RPC service 'gdrivejohnbisync' not found. in \/usr\/share\/php\/openmediavault\/rpc\/rpc.inc:116\nStack trace:\n#0 \/usr\/sbin\/omv-engined(546): OMV\\Rpc\\Rpc::call()\n#1 {main}"}}
```
### /usr/share/openmediavault/workbench/dashboard.d/omv-gdrive-john-bisync-dashboard.yaml
- So that the dashboard is functional, you must observe the following rules:
  - If you have modified gdrivejohnbisync.inc, the service field must contain the exact name of the RPC procedure. 
  - The ID must be different for each plugin.
  - With this ID value, you can install the dashboard on several different NAS units.
  - To generate a new ID, run the following commands:
    - apt install uuid-runtime
    - uuidgen
- To test this plugin, you can keep this value for the ID.
- Create /usr/share/openmediavault/workbench/dashboard.d/omv-gdrive-john-bisync-dashboard.yaml with the content displayed by clicking the link below.

[omv-gdrive-john-bisync-dashboard.yaml](examples/omv-gdrive-john-bisync/usr/share/openmediavault/workbench/dashboard.d/omv-gdrive-john-bisync-dashboard.yaml)

- After creating the file, You need to create the other files so that the plugin appears in the OpenMediaVault GUI.

### /usr/share/openmediavault/workbench/component.d/omv-services-gdrivejohnbisync-status-page.yaml
- So that the component is functional, you must observe the following rules:
  - If you have modified gdrivejohnbisync.inc, the service field must contain the exact name of the RPC procedure.
  - If you have changed the plugin name, update the `name`, `id` and 'title' fields.
- Create /usr/share/openmediavault/workbench/component.d/omv-services-gdrivejohnbisync-status-page.yaml with the content displayed by clicking the link below.

[omv-services-gdrivejohnbisync-status-page.yaml](examples/omv-gdrive-john-bisync/usr/share/openmediavault/workbench/component.d/omv-services-gdrivejohnbisync-status-page.yaml)

### /usr/share/openmediavault/workbench/navigation.d/services.gdrivejohnbisync.yaml
- So that the component is functional, you must observe the following rules:
  - If you have changed the plugin name, update the `path`, `text` and 'url' fields.

- Create /usr/share/openmediavault/workbench/navigation.d/services.gdrivejohnbisync.yaml with the content displayed by clicking the link below.

[navigation.d/services.gdrivejohnbisync.yaml](examples/omv-gdrive-john-bisync/usr/share/openmediavault/workbench/navigation.d/services.gdrivejohnbisync.yaml)








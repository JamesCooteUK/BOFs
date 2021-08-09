# BOFs
Collection of BOFs for Cobalt Strike

## Create world read/writeable folder
```
beacon> sharefolder_create C:\james SHARE
[*] Create world read/writeable folder (@jkcoote)
[+] host called home, sent: 1305 bytes
[+] received output:
Folder successfully created: C:\james
[+] received output:
Folder successfully shared
```

```
beacon> sharefolder_delete C:\james
[*] Deleting folder (@jkcoote)
[+] host called home, sent: 1293 bytes
[+] received output:
Folder successfully deleted: C:\james
```
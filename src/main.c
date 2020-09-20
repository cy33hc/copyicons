#include <debugnet.h>
#include <stdio.h>

#include "main.h"
#include "init.h"
#include "pfs.h"
#include "file.h"

#define ip_server "192.168.100.14"
#define port_server 18194
#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    debugNetInit(ip_server,port_server,DEBUG);

	initCopyIcons();

	SceUID dfd = sceIoDopen("ux0:app");
	if (dfd < 0)
		sceKernelExitProcess(0);

	int res = 0;
	char game_path[512];
	char pfs_path[512];
	char icon_path[512];
	char meta_icon_path[512];
	char pic_path[512];
	char meta_pic_path[512];
	char meta_path[512];

	do {
		SceIoDirent dir;
		memset(&dir, 0, sizeof(SceIoDirent));

		res = sceIoDread(dfd, &dir);
		if (res > 0) {
			sprintf(game_path, "ux0:app/%s", dir.d_name);
			sprintf(meta_path, "ur0:appmeta/%s", dir.d_name);
			sprintf(pfs_path, "ux0:app/%s/sce_pfs", dir.d_name);
			sprintf(icon_path, "ux0:app/%s/sce_sys/icon0.png", dir.d_name);
			sprintf(meta_icon_path, "ur0:appmeta/%s/icon0.png", dir.d_name);
			sprintf(pic_path, "ux0:app/%s/sce_sys/pic0.png", dir.d_name);
			sprintf(meta_pic_path, "ur0:appmeta/%s/pic0.png", dir.d_name);

			debugNetPrintf(DEBUG, "=============== %s ===============\n", dir.d_name);
			debugNetPrintf(DEBUG, "game_path: %s\n", game_path);
			debugNetPrintf(DEBUG, "pfs_path: %s\n", pfs_path);
			debugNetPrintf(DEBUG, "icon_path: %s\n", icon_path);
			debugNetPrintf(DEBUG, "meta_icon_path: %s\n", meta_icon_path);
			debugNetPrintf(DEBUG, "pic_path: %s\n", pic_path);
			debugNetPrintf(DEBUG, "meta_pic_path: %s\n", meta_pic_path);
			
			int pfs_path_exists = checkFolderExist(pfs_path);
			int icon_path_exists = checkFileExist(icon_path);
			int meta_icon_path_exists = checkFileExist(meta_icon_path);
			int pic_path_exists = checkFileExist(pic_path);
			int meta_pic_path_exists = checkFileExist(meta_pic_path);

			debugNetPrintf(DEBUG, "pfs_path_exists: %d\n", pfs_path_exists);
			debugNetPrintf(DEBUG, "icon_path_exists: %d\n", icon_path_exists);
			debugNetPrintf(DEBUG, "meta_icon_path_exists: %d\n", meta_icon_path_exists);
			debugNetPrintf(DEBUG, "pic_path_exists: %d\n", pic_path_exists);
			debugNetPrintf(DEBUG, "meta_pic_path_exists: %d\n", meta_pic_path_exists);

			if (pfs_path_exists && ((icon_path_exists && !meta_icon_path_exists) || (pic_path_exists && !meta_pic_path_exists)))
			{
				createDirectory(meta_path);
				pfsMount(game_path);
				if (icon_path_exists && !meta_icon_path_exists)
				{
					copyFile(icon_path, meta_icon_path, NULL);
					debugNetPrintf(DEBUG, "copied %s to %s\n", icon_path, meta_icon_path);
				}

				if (pic_path_exists && !meta_pic_path_exists)
				{
					copyFile(pic_path, meta_pic_path, NULL);
					debugNetPrintf(DEBUG, "copied %s to %s\n", pic_path, meta_pic_path);
				}
				pfsUmount();
			}
			else if ((icon_path_exists && !meta_icon_path_exists) || (pic_path_exists && !meta_pic_path_exists))
			{
				createDirectory(meta_path);
				if (icon_path_exists && !meta_icon_path_exists)
				{
					copyFile(icon_path, meta_icon_path, NULL);
					debugNetPrintf(DEBUG, "copied %s to %s\n", icon_path, meta_icon_path);
				}

				if (pic_path_exists && !meta_pic_path_exists)
				{
					copyFile(pic_path, meta_pic_path, NULL);
					debugNetPrintf(DEBUG, "copied %s to %s\n", pic_path, meta_pic_path);
				}
			}
		}
	} while (res > 0);

	finishCopyIcons();
	sceKernelExitProcess(0);
	return 0;
}

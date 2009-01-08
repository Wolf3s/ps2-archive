#ifndef _FAT_DRIVER_H
#define _FAT_DRIVER_H 1

#include "fat.h"

typedef struct _fat_driver {
	mass_dev* dev;
	fat_bpb  partBpb;	//partition bios parameter block

	// modified by Hermes
#define MAX_DIR_CLUSTER 512
	unsigned int cbuf[MAX_DIR_CLUSTER]; //cluster index buffer // 2048 by Hermes

	unsigned int  lastChainCluster;
	int lastChainResult;

	// fat_write
	unsigned char tbuf[512]; //temporary buffer

/* enough for long filename of length 260 characters (20*13) and one short filename */
#define MAX_DE_STACK 21
	unsigned int deSec[MAX_DE_STACK]; //direntry sector
	int          deOfs[MAX_DE_STACK]; //direntry offset
	int          deIdx; //direntry index

#define SEQ_MASK_SIZE 2048         //Allow 2K files per directory
	u8 seq_mask[SEQ_MASK_SIZE/8];      //bitmask for consumed seq numbers
#define DIR_MASK_SIZE 2048*11      //Allow 2K maxed fullnames per directory
	u8 dir_used_mask[DIR_MASK_SIZE/8]; //bitmask for used directory entries

#define MAX_CLUSTER_STACK 128
	unsigned int clStack[MAX_CLUSTER_STACK]; //cluster allocation stack
	int clStackIndex;
	unsigned int clStackLast; // last free cluster of the fat table
} fat_driver;

typedef struct _fat_dir_list {
	unsigned int  direntryCluster; //the directory cluster requested by getFirstDirentry
	int direntryIndex; //index of the directory children
} fat_dir_list;

void fat_mountCheck(void);
int fat_mount(mass_dev* dev, part_record* rec);
void fat_forceUnmount(fat_driver* fatd); //dlanor: added for disconnection events (flush impossible)
void fat_setFatDirChain(fat_driver* fatd, fat_dir* fatDir);
int fat_readFile(fat_driver* fatd, fat_dir* fatDir, unsigned int filePos, unsigned char* buffer, int size);
int fat_getFirstDirentry(fat_driver* fatd, char * dirName, fat_dir_list* fatdlist, fat_dir* fatDir);
int fat_getNextDirentry(fat_driver* fatd, fat_dir_list* fatdlist, fat_dir* fatDir);

fat_driver * fat_getData(int device);
int      fat_getFileStartCluster(fat_driver* fatd, const char* fname, unsigned int* startCluster, fat_dir* fatDir);
int      fat_getDirentrySectorData(fat_driver* fatd, unsigned int* startCluster, unsigned int* startSector, int* dirSector);
int      fat_getDirentry(fat_direntry_sfn* dsfn, fat_direntry_lfn* dlfn, fat_direntry* dir );
int      fat_getClusterChain(fat_driver* fatd, unsigned int cluster, unsigned int* buf, int bufSize, int start);
void     fat_invalidateLastChainResult(fat_driver* fatd);
void     fat_getClusterAtFilePos(fat_driver* fatd, fat_dir* fatDir, unsigned int filePos, unsigned int* cluster, unsigned int* clusterPos);

#endif

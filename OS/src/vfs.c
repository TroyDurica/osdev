#include "vfs.h"
#include "terminal.h"
#include "log.h"
#include "common.h"

fs_node_t *fs_root = 0;

uint32_t read_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
    if (node->read != 0)
        return node->read(node, offset, size, buffer);
    else
        return 0;
}

uint32_t write_fs(fs_node_t *node, uint32_t offset, uint32_t size, uint8_t *buffer)
{
    if (node->write != 0)
        return node->write(node, offset, size, buffer);
    else
        return 0;
}

void open_fs(fs_node_t *node, uint8_t read, uint8_t write)
{
    if (node->open != 0)
        return node->open(node);
}

void close_fs(fs_node_t *node)
{
    if (node->close != 0)
        return node->close(node);
}

struct dirent *readdir_fs(fs_node_t *node, uint32_t index)
{
    if ((node->flags&0x7) == FS_DIRECTORY &&
        node->readdir != 0 )
        return node->readdir(node, index);
    else
        return 0;
}

fs_node_t *finddir_fs(fs_node_t *node, char *name)
{
    if ((node->flags&0x7) == FS_DIRECTORY &&
        node->finddir != 0 )
        return node->finddir(node, name);
    else
        return 0;
}

void list_vfs()
{
    asm("cli");
	uint8_t a[0x1000];

	for (int i = 0; i < 0x1000; i++)
	{
		a[i] = 0x20;
	}
	int i = 0;
	struct dirent *node = 0;

	while ( (node = readdir_fs(fs_root, i)) != 0)
	{
		printk("Found file %s", node->name);
		log(LOG_INFO, "Found file %s", node->name);
		fs_node_t *fsnode = finddir_fs(fs_root, node->name);

		if ((fsnode->flags&0x7) == FS_DIRECTORY)
		{
			printk("\n\t(directory)\n");
			log(LOG_INFO, "\n\t(directory)\n");
		}
		else
		{
			printk("\n\t size: %d Bytes\n", fsnode->length);
			log(LOG_INFO, "\n\t size: %d Bytes\n", fsnode->length);
			printk("\tcontents:\n");
			log(LOG_INFO, "\tcontents:\n");
			char buf[256];
			uint32_t sz = read_fs(fsnode, 0, 256, buf);
			int j;
			for (j = 0; j < sz; j++)
			{
				printk("%c", buf[j]);
				//log(LOG_NPRE, "%c", buf[j]);
			}

			printk("\n");
			log(LOG_NPRE, "\n");
		}
		i++;
	}
	printk("\n");
	log(LOG_NPRE, "\n");

	asm("sti");
}

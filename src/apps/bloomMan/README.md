## bloomMan

***
**Note:** This tool is available through [our website](http://quickblocks.io). Contact us at [sales@greathill.com](mailto:sales@greathill.com) for more information.
***

#### Usage

`Usage:`    bloomMan [-s|-c|-r|-c|-v|-h] lists  
`Purpose:`  Work with QBlocks-style bloom filters and/or present statistics.  
`Where:`  

| Short Cut | Option | Description |
| -------: | :------- | :------- |
|  | lists | list of either account address, blocks, or both (if both, show hits statistics) |
| -s | --stats | include statistics about blooms in the range |
| -c | --combine | visit each bloom folder and combine each bloom into a single file by block number |
| -r | --rewrite | re-write the given bloom(s) -- works only with block numbers |
| -c | --check | check that a bloom contains all addresses in the given block(s) |
| -v | --verbose | set verbose level. Either -v, --verbose or -v:n where 'n' is level |
| -h | --help | display this help screen |

#### Other Options

All **QBlocks** command-line tools support the following commands (although in some case, they have no meaning):

    Command     |     Description
    -----------------------------------------------------------------------------
    --version   |   display the current version of the tool
    --nocolor   |   turn off colored display
    --wei       |   specify value in wei (the default)
    --ether     |   specify value in ether
    --dollars   |   specify value in US dollars
    --file:fn   |   specify multiple sets of command line options in a file.

<small>*For the `--file:fn` option, place a series of valid command lines in a file and use the above options. In some cases, this option may significantly improve performance. A semi-colon at the start of a line makes that line a comment.*</small>

**Powered by Qblocks<sup>&trade;</sup>**



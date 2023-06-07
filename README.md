# Slurm-SPANK-Examples
A series of Slurm SPANK plugin examples to help people new to writing them. Hopefully it will help understand, and maybe dare I say inspire.

## Installation
The only things required to build the plugin is GCC and the spank.h header file. The header file can be located in the slurm-devel package.

Assuming the source code is demo.c. To compile run ...
```bash
gcc -fPIC -shared -o demo.so demo.c
```
This should generate a shared object called demo.so containing all your hard work. This now needs to be copied onto each compute and login node into the desired location as specified in the plugstack.conf file.
```bash
optional <PATH>/demo.so
```
If you don't specify the full path then the PluginDir in slurm.conf will be searched.

Note: Alternatively, you can create a plugstack.conf.d directory and point to it via an include statement in the plugstack.conf. Then copy the supplied demo.conf examples into the plugstack.conf.d directory.

Rather than using the 'optional' argument you can specify 'required'. I tend to use 'optional' when developing a plugin, as the 'required' argument will place the node into a drain state should a function within the plugin fail - while this sounds bad it can be helpful if you are using the SPANK plugin to perform node checks.

## License
[MIT](https://github.com/UEA-RSCS/Slurm-SPANK-Examples/blob/main/LICENSE)
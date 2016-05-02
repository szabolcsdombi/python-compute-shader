# Python ComputeShader

The main idea is to simpify the process of coding a compute shader in OpenGL for python This module has no ability to handle image2D, Uniform Buffers, etc...<br>
The idea is to fill an SSBO with data, run a compute shader on it, then read the SSBO's content.<br>
One can use multiple buffers for the communication, there is no optimization possibilities yet for read-only and write-only buffers.<br>

# Requirements

> Windows platform<br>
> ![OpenGL 4.3+](https://raw.githubusercontent.com/cprogrammer1994/Python-ComputeShader/master/Docs/OpenGL43.png)<br>
> ![Python 3.3](https://raw.githubusercontent.com/cprogrammer1994/Python-ComputeShader/master/Docs/Python33.png)<br>
> ![Python 3.4](https://raw.githubusercontent.com/cprogrammer1994/Python-ComputeShader/master/Docs/Python34.png)<br>
> ![Python 3.5](https://raw.githubusercontent.com/cprogrammer1994/Python-ComputeShader/master/Docs/Python35.png)<br>

-----------------------------------

# Common

```
Info()
StartTimer()
GetTimer()
```

> Info returns a python dict filled with glGet<br>
 - GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS<br>
 - GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS<br>
 - GL_MAX_COMPUTE_SHARED_MEMORY_SIZE<br>
 - GL_MAX_COMPUTE_ATOMIC_COUNTERS<br>
 - GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS<br>
 - GL_MAX_COMPUTE_WORK_GROUP_COUNT<br>
 - GL_MAX_COMPUTE_WORK_GROUP_SIZE<br>
 - GL_MAX_UNIFORM_BLOCK_SIZE<br>
 - GL_MAX_COMPUTE_UNIFORM_BLOCKS<br>
 - GL_MAX_COMPUTE_UNIFORM_COMPONENTS<br>
 - GL_MAJOR_VERSION<br>
 - GL_MINOR_VERSION<br>
 
> GetTimer returns a float in seconds<br>

# Compute Shader (CS)

```
NewCS(source)
UseCS(cs, x, y, z)
DeleteCS(cs)
```

> NewCS returns a new compute shader compiled from the source<br>
> rest of the functions return None<br>

# Shader Storage Buffer Object (SSBO)

```
NewSSBO(content)
UseSSBO(ssbo, cs, binding)
UpdateSSBO(ssbo, data, offset)
ReadSSBO(ssbo, size, offset)
DeleteSSBO(ssbo)
```

> NewSSBO returns a new compute shader initialized with the content<br>
> ReadSSBO returns the value read from the GPU starting at the offset<br>
> rest of the functions return None<br>

-----------------------------------

# Optional parameters

```
NewCS(source)
UseCS(cs, [x = 1, y = 1, z = 1])
DeleteCS(cs)
NewSSBO(content)
UseSSBO(ssbo, cs, [binding = 0])
UpdateSSBO(ssbo, data, [offset = 0])
ReadSSBO(ssbo, size, [offset = 0])
DeleteSSBO(ssbo)
```

# Exceptions

There is a poor implementation for exceptions inside the module but they are hard to catch since the 32 and 64 bit version of this module has it's own Exception


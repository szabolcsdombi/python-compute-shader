# Python-ComputeShader

# Requirements

> Windows platform<br>
> OpenGL 4.3<br>
> Python 3.5<br>

-----------------------------------

# Compute Shader (CS)

```
NewCS(source)
UseCS(cs, x, y, z)
DeleteCS(cs)
```

# Compute Shader Storage Buffer Object (SSBO)

```
NewSSBO(content)
UseSSBO(ssbo, cs, name)
UpdateSSBO(ssbo, data, offset)
ReadSSBO(ssbo, size, offset)
DeleteSSBO(ssbo)
```

# Description

The main idea is to simpify the process of coding a compute shader in OpenGL for python This module has no ability to handle image2D, Uniform Buffers, etc...
The idea is to fill an SSBO with data, run a compute shader on it, then read the SSBO's content.
One can use multiple buffers for the communication, there is no optimization possibilities yet for read-only and write-only buffers.

-----------------------------------

# Optional parameters

```
NewCS(source)
UseCS(cs, [x = 1, y = 1, z = 1])
DeleteCS(cs)
NewSSBO(content)
UseSSBO(ssbo, cs, name)
UpdateSSBO(ssbo, data, [offset = 0])
ReadSSBO(ssbo, size, [offset = 0])
DeleteSSBO(ssbo)
```

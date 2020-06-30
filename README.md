C++ Libraries by Róna Balázs
============================

- [C++ Libraries by Róna Balázs](#c-libraries-by-róna-balázs)
  - [Introduction](#introduction)
  - [Projects](#projects)
  - [Namespaces](#namespaces)
  - [Building a project](#building-a-project)
  - [Dependencies](#dependencies)
  - [Compatibility](#compatibility)

Introduction
---

Hi, I am Balázs and this is a repository for some of my libraries!

There are numerous projects that I am woking on parallel, such as cryptography, intrinsics and other utilities.
My aim is for my projects to be absolutely perfect without any warnings, bugs or missing features.
It is also my intention to write meaningful comments and documentation to make every bit of my code clear and understandable for everyone, but these might only come in later commits...

Any kind of contribution is welcome! AMA, I am more than happy to answer!

Projects
---

| Project                  | Description                                                                                                                                                                         |
| :----------------------- | :---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Iterator                 | Basic iterator template. (The standard `iterator` was declared [deprecated in C++17](https://en.cppreference.com/w/cpp/iterator/iterator)                                           |
| Matrix                   | Advanced matrix class with every essential operation and function.                                                                                                                  |
| Vector                   | Advanced mathematical vector class (not standard `vector`) with every essential operation and function.                                                                             |
| Transform                | Transformations using vectors and matrices.                                                                                                                                         |
| Intrinsics               | [AVX](https://en.wikipedia.org/wiki/Advanced_Vector_Extensions), [SSE](https://en.wikipedia.org/wiki/Streaming_SIMD_Extensions) and other intrinsic function wrappers.              |
| Base Transcoder          | Base transcoder template. (Currently only working with powers of 2)                                                                                                                 |
| MD5, RIPEMD160, SHA1/2/3 | [Cryptographic hash function](https://en.wikipedia.org/wiki/Cryptographic_hash_function) implementations.                                                                           |
| HMAC                     | [HMAC](https://en.wikipedia.org/wiki/HMAC) template implementation.                                                                                                                 |
| AES                      | [AES](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard) implementation with different [modes of operation](https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation). |
| Byte Concatenation       | *Endian-specific* integer concatenation algorithms.                                                                                                                                 |
| Byte Swap                | Integer byte swap algorithm.                                                                                                                                                        |
| Endian                   | *Endian-specific* memory read/write algorithms.                                                                                                                                     |
| Rotate                   | Bitwise left/right rotate algorithms.                                                                                                                                               |

Namespaces
---

My projects are organized in different namespaces. The main namespace is `rb` (my monogram).
Different projects may share a namespace if their subjects are similar.

| Namespace | Projects                                                            |
| :-------- | :------------------------------------------------------------------ |
| `iter`    | Iterator                                                            |
| `math`    | Matrix, Vector, Transform                                           |
| `intrin`  | Intrinsics                                                          |
| `crypto`  | Base Transcoder, MD5, RIPEMD160, SHA1, SHA2, SHA3/Keccak, HMAC, AES |
| `bit`     | Byte Concatenation, Byte Swap, Endian, Rotate                       |

I also define literals in the `literals` namespace.

Building a project
---

I use [Premake](https://premake.github.io/), a project generator tool to help you set up the workspace for your system and IDE.
You can download Premake 5.0 [here](https://premake.github.io/download.html#v5)!

1. Place premake software into the premake folder where its license is located. (`..\RBLibs\premake\`)
2. Open command prompt/terminal.
3. Navigate to the repository. (`..\RBLibs\`)
4. Run premake and choose your build action. (On windows, using Visual Studio 2019: `.\premake\premake.exe vs2019`)
5. Your project should be ready to use in the `build` folder!

See the [Premake User Guide](https://github.com/premake/premake-core/wiki/Using-Premake) for more help!

Dependencies
---

*No dependencies yet!*

Compatibility
---

- I use [Microsoft Unit Test Framework](https://docs.microsoft.com/en-us/visualstudio/test/how-to-use-microsoft-test-framework-for-cpp?view=vs-2019) for testing.
- Some intrinsic headers or functions may not be available on all machines.
- *Any other stuff that I did not think of...*

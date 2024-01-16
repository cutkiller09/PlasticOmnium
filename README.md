# Project Code Generator

## Description

This tool, aims to provide an automatic project code skeleton generator. 
It is based on Software Engineering Process group of Plastic Omnium

## Table of Contents  

README is divided in paragraph :

- [Installation](INSTALL.md) 
 
## Software Engineering Process group

![SWE](images/SWE.PNG)

The Software Architecture contains in Software Architectural Design (SWAD) is composed of :
   - Software Elements : Result of the decomposition of the architecture on software level: The software is decomposed into elements of the software architecture across appropriate hierarchical levels down to the software components (the lowest level elements of the software architecture).
   - Software Components : Software-Components (SW-C) are architectural elements that provide and/or require interfaces and are connected to each other through the Virtual Functional Bus to fulfill architectural responsibilities. The Software Component is the central structural element used when building a system at the VFB-level. A SW-C has well-defined ports, through which the component can interact with other Software Components.
A Software Component has a formal description defined by the Software Component template. Software Components can be abbreviated as SW-Cs.
A SW-C contains a Software Component Description (SW-CD) and the implementation.
In Automotive SPICE V3.0 the term "software component" is used for the lowest level elements of the software architecture for which finally the detailed design is defined. A software "component" consists of one or more software "units".

The Software Detailed Design Component is a specific detailed design for a component and is composed of :
    - Units : Part of a software component which is not further subdivided. 

![SWE](images/SWE2.PNG)


## Project Architecture

```mermaid 
flowchart LR
subgraph "Input "
    A[Architecture.c] --> B(fa:fa-spinner)
    G[Templates ...] --> B(fa:fa-spinner)
    H[Cartouches ...] --> B(fa:fa-spinner)
  end
    
    B --> |script_architecture.sh| C[03_Project_Skeleton]
subgraph "Output automatically generated"
    C --> D[01_Elements]
    C --> E[02_Components]
    C --> F[03_Settings]
    D --> I[01_SWE1_Specification]
    D --> J[02_SWE2_Architecture]
    D --> K[03_SWE5_Integration_Tests]
    D --> L[04_SWE6_Qualification_Tests]
    E --> M[01_HighLevel]
    E --> N[02_Middleware]
    E --> O[03_Driver] 
    M --> Q[...] 
    N --> S[Component 1]
    N --> T[...]
    N --> U[Component N] 
    O --> W[...] 
    S --> V[doc]
    S --> X[inc]
    S --> Y[src]
    S --> Z[test]
    S --> AA[CMakeLists.txt]
    S --> AB[CoverageReport.sh]
    S --> AC[Doxyfile.in]
    S --> AD[Readme.md]
    
  end
  
    style A fill:#b5e61d,stroke:#11,stroke-width:4px
```

He will needs in input an "Architecture.c" file with all your compents and units inside in this format (use it as an example) :
Please be aware : spaces at end of lines are forbidden

```bash
#Product
#$NAME | $PROJECT_ID | $OEM
INVERTER | 1272489 | IVECO

#Component_HighLevel
#$NAME
upg_app

#Units
#$UNIT | $FUNCTION  | $PARAMETER  | $ROLE 
Application | Init_Hook  | void
Application | Background_Hookgure | void
#FinComponent

#Component_Middleware
#$NAME
Aux

#Units
#$UNIT | $FUNCTION  | $PARAMETER | $ROLE 
TS32|s32DAux_eInit_Exe|void
TS32|s32DAux_eReloadInternal_Exe|TU32 Fu32MagicNumber 
#FinComponent

#Component_Driver
#$NAME
Drv_Can

#Units
#$UNIT | $FUNCTION  | $PARAMETER | $ROLE 
DCan | eInit_Exe| TU16 Fu16CtrlId
DCan | eConfigBaudRate_Exe|TU16 Fu16CtrlId,TU16 Fu16Baudrate 
#FinComponent
```

Each component is sub-divided in sections

```mermaid 
flowchart LR 
    A[Component]  --> B[doc]
    B --> C[SWDD_Productname_Oem_ProductId.docx]
    B --> D[Doxygen]
    A --> XC[inc]
    A --> Y[Src]
    A --> Z[test]
    A --> AA[CMakeLists.txt]
    A --> AB[CoverageReport.sh]
    A --> AC[Doxyfile.in]
    A --> AD[Readme.md]
    Z --> AE[01_SWE4_Units_Tests]
    Z --> AJ[libs]
    AE --> AI[doc]
    AE --> AF[inc]
    AE --> AG[src]
    AE --> AH[stubs]
    AI --> E[coverage]
    AI --> F[Cartouche_SWE4.docx]
    AI --> G[SWE4_Checklist_Review_Productname_Oem_ProductId.xlsm]
    AI --> H[SWE4_Units_Tests_Plan_Productname_Oem_ProductId.xlsm]

    style G fill:#b5e61d,stroke:#11,stroke-width:4px
    style H fill:#b5e61d,stroke:#11,stroke-width:4px 
    style E fill:#b5e61d,stroke:#11,stroke-width:4px 
    style C fill:#b5e61d,stroke:#11,stroke-width:4px
```

Coverage Folder will have all coverage results for 
![Screenshot](images/Capture.PNG)

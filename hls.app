<project xmlns="com.autoesl.autopilot.project" name="PE_MERGE" top="PE_ComputeUnit_1">
    <includePaths/>
    <libraryPaths/>
    <Simulation>
        <SimFlow name="csim" csimMode="0" lastCsimMode="0"/>
    </Simulation>
    <files xmlns="">
        <file name="../PE_Compute.h" sc="0" tb="1" cflags=" -Wno-unknown-pragmas" csimflags=" -Wno-unknown-pragmas" blackbox="false"/>
        <file name="../main_PE.cpp" sc="0" tb="1" cflags=" -Wno-unknown-pragmas" csimflags=" -Wno-unknown-pragmas" blackbox="false"/>
        <file name="PE_MERGE/PE.cpp" sc="0" tb="false" cflags="" csimflags="" blackbox="false"/>
    </files>
    <solutions xmlns="">
        <solution name="merge_ap_int8" status="inactive"/>
        <solution name="CU__ap_int8" status="inactive"/>
        <solution name="CU_HLS_stream" status="active"/>
    </solutions>
</project>


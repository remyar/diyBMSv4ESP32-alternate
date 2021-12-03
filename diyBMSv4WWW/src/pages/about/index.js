import React, { useState, useEffect } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

function AboutPage(props) {
    return <div>
        <h1>About</h1>
        <div className="region">
            <h2 id="ap1">Source Code &amp; Hardware</h2>
            <p>Code:<a href="https://github.com/stuartpittaway/diyBMSv4ESP32" rel="noreferrer"
                target="_blank">https://github.com/stuartpittaway/diyBMSv4ESP32</a></p>
            <p>Hardware: <a href="https://github.com/stuartpittaway/diyBMSv4" rel="noreferrer"
                target="_blank">https://github.com/stuartpittaway/diyBMSv4</a></p>
            <h2 id="ap2">Videos</h2>
            <a id="b13" href="https://youtube.com/playlist?list=PLHhwQCDPuRcZW3u0jJucsiCCsUbNbMy-c" rel="noreferrer"
                target="_blank">YouTube videos on installation and configuration</a>
            <h2 id="ap3">Patreon</h2>
            <p id="ap4">Remember, this product is free for personal use, if you would like to make a regular donation to
                keep the
                features and improvements flowing, use the Patreon link below. Even just a coffee/beer a month makes a
                difference. Thank you!
                <a href="https://www.patreon.com/StuartP" rel="noreferrer" target="_blank"><img src="patron.png"
                    width="176" height="34" alt="Patron" /></a>
            </p>
        </div>
        <div className="region">
            <h2 id="ap5">WARNING</h2>
            <p id="ap6">This is a DIY product/solution so don’t use this for safety critical systems or in any situation
                where
                there could be a risk to life.</p>
            <p id="ap7">There is no warranty, it may not work as expected or at all.</p>
            <p id="ap8">The use of this project is done so entirely at your own risk. It may involve electrical voltages
                which
                could kill - if in doubt, seek help.</p>
            <p id="ap8a">The use of this project may not be compliant with local laws or regulations - if in doubt, seek
                help.</p>
        </div>
        <div className="region">
            <h2 id="ap9">License</h2>
            <p id="ap10">This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 2.0 UK:
                England &
                Wales License.</p>
            <a href="https://creativecommons.org/licenses/by-nc-sa/2.0/uk/" rel="noreferrer"
                target="_blank">https://creativecommons.org/licenses/by-nc-sa/2.0/uk/</a>
        </div>
        <div className="region">
            <h2 id="ap11">Platform &amp; Version</h2>
            <p id="hn">Host name:<span id="HostName"></span></p>
            <p>Processor: %PLATFORM%</p>
            <p>Version: <a href="https://github.com/stuartpittaway/diyBMSv4ESP32/commit/%GIT_VERSION%" rel="noreferrer"
                    target="_blank">%GIT_VERSION%</a></p>
            <p>Compiled: %COMPILE_DATE_TIME%</p>
            <p>Language: <span id='lang'>language</span></p>
            <p id="sdk">SDK Version:<span id="SdkVersion"></span></p>
            <p id="mfheap">Min free Heap:<span id="MinFreeHeap"></span></p>
            <p id="fheap">Free heap:<span id="FreeHeap"></span></p>
            <p id="heapz">Heap size:<span id="HeapSize"></span></p>
        </div>
    </div>;
}

export default withStoreProvider(withNavigation(injectIntl(AboutPage)));
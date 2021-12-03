import React, { useState, useEffect } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

function StoragePage(props) {

    const storage = props.globalState.storage || {};

    return <div>
        <h1>Storage</h1>
        <div className="region">
            <h2>SD Card</h2>
            <p id='sdcardmissing' class="warning" style={{display : storage.sdcard?.available ? "none" : "block"}}>SD card not installed</p>
            Used <span id="sdcard_used">{Number(storage.sdcard.used).toLocaleString()}</span>KiB (<span id="sdcard_used_percent">{((storage.sdcard.used / storage.sdcard.total) * 100).toFixed(1)}</span>%) of <span
                id="sdcard_total">{Number(storage.sdcard.total).toLocaleString()}</span>KiB
            <p>
                <button type="button" id="unmount">Unmount</button>
                <button type="button" id="mount">Mount</button>
                <button type="button" id="savewifi">Save Wifi</button>
                <button type="button" id="saveconfig">Save Configuration</button>
            </p>
            <ul id="sdcardfiles">
                {storage.sdcard.files && storage.sdcard.files.map((file , index) => {
                    return file && <li>
                                <a href={"download?type=sdcard&file=" + encodeURI(file)} >{file}</a>
                            </li>
                })}
            </ul>
        </div>
        <div className="region">
            <h2>Flash Memory</h2>
            <p>Used <span id="flash_used">{Number(storage.flash.used).toLocaleString()}</span>KiB (<span id="flash_used_percent">{((storage.flash.used / storage.flash.total) * 100).toFixed(1)}</span>%) of <span
                    id="flash_total">{Number(storage.flash.total).toLocaleString()}</span>KiB</p>
            <ul id="flashfiles"></ul>
        </div>
        <div className="region">
            <h2>Logging</h2>
            <p id="b12">Cell data and output states can be stored as log files using an SD card.</p>
            <form id="loggingForm" method="POST" action="savestorage.json" autocomplete="off">
                <div class="settings">
                    <div>
                        <label for="loggingEnabled">SD card logging enabled</label>
                        <input type="checkbox" name="loggingEnabled" id="loggingEnabled" checked={storage.logging ? true : false} />
                    </div>
                    <div>
                        <label for="loggingFreq">Cell data logging frequency (seconds)</label>
                        <select name="loggingFreq" id="loggingFreq" defaultValue={storage.frequency}>
                            <option>15</option>
                            <option>30</option>
                            <option>60</option>
                            <option>90</option>
                            <option>120</option>
                            <option>300</option>
                        </select>
                    </div>
                    <button type="submit">Save logging settings</button>
                </div>
            </form>
        </div>
    </div>
}

export default withStoreProvider(withNavigation(injectIntl(StoragePage)));
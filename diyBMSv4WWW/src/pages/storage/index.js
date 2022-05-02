import React, { useState, useEffect } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';
import { withSnackBar } from '../../providers/snackBar';

import actionsBms from '../../actions/bms';

import { saveAs } from "file-saver";

function StoragePage(props) {

    const storage = props.globalState.storage || {};
    const snackbar = props.snackbar;
    const intl = props.intl;

    const [storageLogging, setStorageLogging] = useState(storage.logging ? true : false);
    const [loggingFreq, setLoggingFreq] = useState(storage.frequency);

    return <div>
        <h1>Storage</h1>
        <div className="region">
            <h2>SD Card</h2>
            <p id='sdcardmissing' className="warning" style={{ display: storage.sdcard?.available ? "none" : "block" }}>SD card not installed</p>
            Used <span id="sdcard_used">{Number(storage.sdcard?.used).toLocaleString()}</span> MB (<span id="sdcard_used_percent">{((storage.sdcard?.used / storage.sdcard?.total) * 100).toFixed(1)}</span>%) of <span
                id="sdcard_total">{Number(storage.sdcard?.total).toLocaleString()}</span> MB
         {/*   <p>
                <button style={{ cursor: 'pointer' }} type="button" id="unmount" onClick={() => {
                    props.dispatch(actionsBms.setSdUnmount()).then(() => {

                    });
                }}>Unmount</button>
                <button style={{ cursor: 'pointer' }} type="button" id="mount" onClick={() => {
                    props.dispatch(actionsBms.setSdMount());
                }}>Mount</button>
                <button style={{ cursor: 'pointer' }} type="button" id="savewifi" onClick={() => {
                    props.dispatch(actionsBms.setSaveWifi());
                }}>Save Wifi</button>
                <button style={{ cursor: 'pointer' }} type="button" id="saveconfig" onClick={() => {
                    props.dispatch(actionsBms.setSaveConfig()).then(() => {
                        snackbar.success(intl.formatMessage({ id: "save.success" }));
                    });
                }}>{intl.formatMessage({ id: "save.configuration" })}</button>
            </p>
            */}
            <ul id="sdcardfiles">
                {storage.sdcard?.files && storage.sdcard?.files.map((file, index) => {
                    return file && <li>
                        <a onClick={async () => {
                            let blob = await props.dispatch(actionsBms.getFile(file));
                            saveAs(blob, file.replace('/',''));
                        }} style={{cursor : 'pointer'}}>{file}</a>
                    </li>
                })}
            </ul>
        </div>
      {/*  <div className="region">
            <h2>Flash Memory</h2>
            <p>Used <span id="flash_used">{Number(storage.flash?.used).toLocaleString()}</span>KiB (<span id="flash_used_percent">{((storage.flash?.used / storage.flash?.total) * 100).toFixed(1)}</span>%) of <span
                id="flash_total">{Number(storage.flash?.total).toLocaleString()}</span>KiB</p>
            <ul id="flashfiles"></ul>
            </div>*/}
        <div className="region">
            <h2>Logging</h2>
            <p id="b12">Cell data and output states can be stored as log files using an SD card.</p>
            <div id="form">
                <div className="settings">
                    <div>
                        <label for="loggingEnabled">SD card logging enabled</label>
                        <input type="checkbox" name="loggingEnabled" id="loggingEnabled" checked={storageLogging} onClick={() => {
                            setStorageLogging(!storageLogging);
                        }} />
                    </div>
                    <div>
                        <label for="loggingFreq">Cell data logging frequency (seconds)</label>
                        <select name="loggingFreq" id="loggingFreq" defaultValue={loggingFreq} onChange={(event) => {
                            setLoggingFreq(event.target.value);
                        }}>
                            <option>15</option>
                            <option>30</option>
                            <option>60</option>
                            <option>90</option>
                            <option>120</option>
                            <option>300</option>
                        </select>
                    </div>
                    <button style={{ cursor: 'pointer' }} onClick={() => {
                        props.dispatch(actionsBms.setStorage({ loggingEnabled: storageLogging ? "on" : "off", loggingFreq: loggingFreq }));
                    }}>Save logging settings</button>
                </div>
            </div>
        </div>
    </div>
}

export default withStoreProvider(withNavigation(withSnackBar(injectIntl(StoragePage))));
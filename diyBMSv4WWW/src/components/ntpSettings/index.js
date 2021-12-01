import React, { useState } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

import actionsBms from '../../actions/bms';


function NtpSettings(props) {

    const settings = props.globalState.settings || {};

    const [NTPServer, setNTPServerName] = useState(settings.NTPServerName);
    const [NTPZoneHour, setTimeZone] = useState(settings.TimeZone);
    const [NTPZoneMin, setMinutesTimeZone] = useState(settings.MinutesTimeZone);

    return <div className="region">
        <h2>Network Time Protocol</h2>
        <p id="b9">Time is set via NTP, if your controller is not connected to the Internet. Time based rules will
            be
            incorrect. This does not automatically correct for daylight saving.</p>
        <p id="b10">Time now is <span id="timenow">{(new Date(1000 * settings.now)).toJSON()}</span></p>
        <div id="form">
            <div className="settings">
                <div>
                    <label for="NTPServer">NTP Server</label>
                    <input type="input" name="NTPServer" id="NTPServer" defaultValue={NTPServer} required="" maxlength="64" onChange={(event) => {
                        setNTPServerName(event.target.value);
                    }} />
                </div>
                <div>
                    <label for="NTPZoneHour">Timezone (hour)</label>
                    <input type="number" min="-23" max="23" step="1" name="NTPZoneHour" id="NTPZoneHour" defaultValue={NTPZoneHour} onChange={(event) => {
                        setTimeZone(event.target.value);
                    }}
                        required="" />
                </div>
                <div>
                    <label for="NTPZoneMin">Timezone (minute)</label>
                    <input type="number" min="0" max="59" step="1" name="NTPZoneMin" id="NTPZoneMin" defaultValue={NTPZoneMin} onChange={(event) => {
                        setMinutesTimeZone(event.target.value);
                    }}
                        required="" />
                </div>
                <div>
                    <label for="NTPDST">Daylight Saving Enabled</label>
                    <input type="checkbox" name="NTPDST" id="NTPDST" defaultValue={settings.DST} />
                </div>
                <button style={{ cursor: 'pointer' }} onClick={() => {
                    props.dispatch(actionsBms.setNtpSettings({ NTPServer, NTPZoneHour, NTPZoneMin }));
                }}>Save NTP settings</button>
            </div>
        </div>
    </div>
}

export default withStoreProvider(withNavigation(injectIntl(NtpSettings)));
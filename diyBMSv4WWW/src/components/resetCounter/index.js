import React, { useState } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

import actionsBms from '../../actions/bms';

function ResetCounter(props) {
    return <div className="region">
        <h2>Reset Counters</h2>
        <div id="form">
            <div className="settings">
                <button style={{ cursor: 'pointer' }} onClick={() => {
                    props.dispatch(actionsBms.setCounter({ resetCounters : 0}));
                }}>Reset counters</button>
            </div>
        </div>
    </div>
}

export default withStoreProvider(withNavigation(injectIntl(ResetCounter)));
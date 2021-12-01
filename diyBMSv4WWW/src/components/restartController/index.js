import React, { useState } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

import actionsBms from '../../actions/bms';

function RestartController(props) {


    return <div className="region">
        <h2>Restart Controller</h2>
        <div id="form">
            <div className="settings">
                <button style={{ sursor: 'pointer' }} onClick={() => {
                    props.dispatch(actionsBms.restartController({ handleRestartController: 0 }));
                }}>Restart Controller</button>
            </div>
        </div>
    </div>
}

export default withStoreProvider(withNavigation(injectIntl(RestartController)));
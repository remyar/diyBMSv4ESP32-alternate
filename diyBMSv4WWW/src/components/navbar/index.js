import React, { useState } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

function NavBar(props) {
    return <div className="header">
        <div className="logocontainer">
            <img className="logo" src="img/logo.png" width="152" height="48" alt="DIYBMS" />
        </div>
        <div className="header-right">
            <div className="notify-container"><a id="home" className="active" href="#home">Home</a></div>
            <div className="notify-container"><a id="modules" href="#modules">Modules</a></div>
            <div className="notify-container"><a id="settings" href="#settings">Settings</a></div>

            <div className="notify-container">
                <span className="notify-bubble" id="activerules"></span>
                <a id="rules" href="#rules">Rules</a>
            </div>

            <div className="notify-container"><a id="more" href="#">More</a></div>
        </div>
    </div>
}

export default withStoreProvider(withNavigation(injectIntl(NavBar)));
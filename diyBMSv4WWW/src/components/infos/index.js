import React, { useState } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

function Infos(props){
    return <div id="info" className="info">
            <div id="sent" className="stat"><span className="x t">Packets sent:</span><span className="x v"></span></div>

        </div>
}

export default withStoreProvider(withNavigation(injectIntl(Infos)));
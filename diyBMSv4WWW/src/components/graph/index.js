import React, { useState } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

import ReactECharts from 'echarts-for-react';

function Graph(props) {
    return <ReactECharts id={props.id || "graph"} option={{}}  style={{width:"100%", height:"100%"}}/>
   // return <div id={props.id || "graph"} style={{width:"100%", height:"100%"}}></div>
}

export default withStoreProvider(withNavigation(injectIntl(Graph)));
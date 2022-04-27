import createAction from '../../middleware/actions';

async function getMonitor2({ getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.get("portcom.json");
        if ( typeof obj == "string"){
            obj = JSON.parse(obj);
        }
        return {
            portcom : obj
        };
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(getMonitor2);
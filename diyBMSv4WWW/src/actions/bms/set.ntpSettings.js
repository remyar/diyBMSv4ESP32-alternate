import createAction from '../../middleware/actions';

async function setNtpSettings(val , { getState , extra }) {
    let api = extra.api;
    try {
        let obj = await api.post("saventp.json",val);
        return obj.success;
    }
    catch(err){
        throw { message: err.message };
    }
}

export default createAction(setNtpSettings);
import {combineReducers} from 'redux';
import AppStateReducer from './reducer-app-state'
import LogReducer from './reducer-log'

const allReducers = combineReducers({
    appState: AppStateReducer,
    log: LogReducer,
    placeholder: function () {return {}}
});

export default allReducers;
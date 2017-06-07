import {combineReducers} from 'redux';
import AppStateReducer from './reducer-app-state'

const allReducers = combineReducers({
    appState: AppStateReducer,
    placeholder: function () {return {}}
});

export default allReducers;
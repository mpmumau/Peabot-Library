import {combineReducers} from 'redux';
import UserReducer from './reducer-users';
import ActiveUserReducer from './reducer-active-user';
import TestReducer from './test_reducer';
import AppStateReducer from './reducer-app-state'

const allReducers = combineReducers({
    users: UserReducer,
    activeUser: ActiveUserReducer,
    testReducer: TestReducer,
    appState: AppStateReducer
});

export default allReducers;
import '../scss/input-range.scss'
import '../scss/styles.scss';

import 'babel-polyfill';
import React from 'react';
import ReactDOM from 'react-dom';
import {Provider} from 'react-redux';
import {createStore} from 'redux';
import allReducers from './reducers/all'
import Layout from './components/layout'

const store = createStore(allReducers);

ReactDOM.render(
    <Provider store={store}>
        <Layout />
    </Provider>, 
    document.getElementById('react-wrapper')
);

document.addEventListener("touchstart", function(){}, true);

//setInterval(function() { console.log("testeroo") }, 1000);
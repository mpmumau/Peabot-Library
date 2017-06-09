import React, {Component} from 'react';
import {bindActionCreators} from 'redux';
import {connect} from 'react-redux';


import '../../scss/log-component.scss';

class LogComponent extends Component {
    constructor(props) {
        super(props);
    }

    render() {
        this.props.log_lines.reverse();
        return (
            <section className="log-component">
                <ul>
                    {this.props.log_lines.map(function(log_line, index){
                        return <li key={index}> {log_line} </li>;
                    })}
                </ul>
            </section>
        );
    }
}

function mapStateToProps(state) {
    var new_props = {};

    new_props.log_lines = state.log.log_lines;

    return new_props;
}

export default connect(mapStateToProps)(LogComponent);
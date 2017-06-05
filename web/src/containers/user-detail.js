import React, {Component} from 'react';
import {connect} from 'react-redux';

class UserDetail extends Component {
    render() {
        if (!this.props.user) {
            return (
                <div>SELECT A USER, motherRFawjk</div>
            );
        }
        return (
            <div>
                <h3>USER DETAIL, BIZNITCH?!?!?!!!</h3>
                <strong>{this.props.user.username}</strong>
                <p>{this.props.user.email}</p>
            </div>
        );
    }
}

function mapStateToProps(state) {
    return {
        user: state.activeUser
    };
}

export default connect(mapStateToProps)(UserDetail);
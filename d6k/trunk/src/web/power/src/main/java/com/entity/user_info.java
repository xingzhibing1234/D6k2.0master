package com.entity;

import java.util.Date;

public class user_info {
    /**
     * This field was generated by MyBatis Generator.
     * This field corresponds to the database column user_info.username
     *
     * @mbggenerated
     */
    private String username;

    /**
     * This field was generated by MyBatis Generator.
     * This field corresponds to the database column user_info.user_pwd
     *
     * @mbggenerated
     */
    private String userPwd;

    /**
     * This field was generated by MyBatis Generator.
     * This field corresponds to the database column user_info.user_auth
     *
     * @mbggenerated
     */
    private String userAuth;

    /**
     * This field was generated by MyBatis Generator.
     * This field corresponds to the database column user_info.view_name
     *
     * @mbggenerated
     */
    private String viewName;

    /**
     * This field was generated by MyBatis Generator.
     * This field corresponds to the database column user_info.create_time
     *
     * @mbggenerated
     */
    private Date createTime;

    /**
     * This method was generated by MyBatis Generator.
     * This method returns the value of the database column user_info.username
     *
     * @return the value of user_info.username
     *
     * @mbggenerated
     */
    public String getUsername() {
        return username;
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method sets the value of the database column user_info.username
     *
     * @param username the value for user_info.username
     *
     * @mbggenerated
     */
    public void setUsername(String username) {
        this.username = username == null ? null : username.trim();
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method returns the value of the database column user_info.user_pwd
     *
     * @return the value of user_info.user_pwd
     *
     * @mbggenerated
     */
    public String getUserPwd() {
        return userPwd;
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method sets the value of the database column user_info.user_pwd
     *
     * @param userPwd the value for user_info.user_pwd
     *
     * @mbggenerated
     */
    public void setUserPwd(String userPwd) {
        this.userPwd = userPwd == null ? null : userPwd.trim();
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method returns the value of the database column user_info.user_auth
     *
     * @return the value of user_info.user_auth
     *
     * @mbggenerated
     */
    public String getUserAuth() {
        return userAuth;
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method sets the value of the database column user_info.user_auth
     *
     * @param userAuth the value for user_info.user_auth
     *
     * @mbggenerated
     */
    public void setUserAuth(String userAuth) {
        this.userAuth = userAuth == null ? null : userAuth.trim();
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method returns the value of the database column user_info.view_name
     *
     * @return the value of user_info.view_name
     *
     * @mbggenerated
     */
    public String getViewName() {
        return viewName;
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method sets the value of the database column user_info.view_name
     *
     * @param viewName the value for user_info.view_name
     *
     * @mbggenerated
     */
    public void setViewName(String viewName) {
        this.viewName = viewName == null ? null : viewName.trim();
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method returns the value of the database column user_info.create_time
     *
     * @return the value of user_info.create_time
     *
     * @mbggenerated
     */
    public Date getCreateTime() {
        return createTime;
    }

    /**
     * This method was generated by MyBatis Generator.
     * This method sets the value of the database column user_info.create_time
     *
     * @param createTime the value for user_info.create_time
     *
     * @mbggenerated
     */
    public void setCreateTime(Date createTime) {
        this.createTime = createTime;
    }
}
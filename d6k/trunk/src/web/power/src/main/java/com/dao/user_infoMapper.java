package com.dao;

import com.entity.user_info;

public interface user_infoMapper {
    /**
     * This method was generated by MyBatis Generator.
     * This method corresponds to the database table user_info
     *
     * @mbggenerated
     */
    int deleteByPrimaryKey(String username);

    /**
     * This method was generated by MyBatis Generator.
     * This method corresponds to the database table user_info
     *
     * @mbggenerated
     */
    int insert(user_info record);

    /**
     * This method was generated by MyBatis Generator.
     * This method corresponds to the database table user_info
     *
     * @mbggenerated
     */
    int insertSelective(user_info record);

    /**
     * This method was generated by MyBatis Generator.
     * This method corresponds to the database table user_info
     *
     * @mbggenerated
     */
    user_info selectByPrimaryKey(String username);

    /**
     * This method was generated by MyBatis Generator.
     * This method corresponds to the database table user_info
     *
     * @mbggenerated
     */
    int updateByPrimaryKeySelective(user_info record);

    /**
     * This method was generated by MyBatis Generator.
     * This method corresponds to the database table user_info
     *
     * @mbggenerated
     */
    int updateByPrimaryKey(user_info record);
}
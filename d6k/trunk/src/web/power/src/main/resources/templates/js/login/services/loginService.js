/**
 * Created by reed on 2017/1/4.
 */
DaQOCloud.factory('loginService',['$http',function ($http) {
    return {
        loginMessage: function (userName,passWord) {

           return $.ajax({
                url : "/userLogin/Login?username="+userName+"&password="+passWord,
                type: "post",
                async: false,
                success:function(result){
                },
                error:function()
                {
                    alert('登录失败，用户名或密码错误！');
                }
            });
        }
    }
}])
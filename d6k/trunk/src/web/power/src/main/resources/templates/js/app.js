/**
 * Created by reed on 2016/12/22.
 */

var DaQOCloud = angular.module('DaQOCloud',[
    'ui.router','ui.bootstrap',
]);

DaQOCloud.run(['$rootScope','$state','$stateParams',function ($rootScope,$state,$stateParams) {
    $rootScope.$state = $state;
    $rootScope.$stateParams = $stateParams;
}])

.config(function ($stateProvider,$urlRouterProvider) {
    $urlRouterProvider
        .otherwise('/login');

    $stateProvider
        //登录页面
        .state('login',{
            url:'/login',
            views:{
                'content':{
                    templateUrl:'html/login/login.html',
                    controller:'loginCtrl'
                }
            }
        })

        // home页面
        .state('home',{
            url:'/home',
            views:{
                'header':{
                    templateUrl:'html/header.html',
                    controller:'headerCtrl'
                },
                'content':{
                    templateUrl:'html/home/amap.html',
                    controller:'homeCtrl'
                },
                'footer':{
                    templateUrl:'html/footer.html'
                }
            }
        })

        //工程监控页面
        .state('project',{
            url:'/project',
            views:{
                'header':{
                    templateUrl:'html/header.html',
                    controller:'headerCtrl'
                },
                'content':{
                    templateUrl:'html/03_project/3pro_first.html',
                    controller:'projectCtrl'
                },
                'footer':{
                    templateUrl:'html/footer.html'
                },
                'projectMenu@project': {
                    templateUrl:'html/03_project/menu/projectMenu.html',
                    controller:'projectMenuCtrl'
                }
            }
        })
        //工程Tab页面
        .state('project.tabs',{
            url:'/:projectId',
            views: {
                'projectTabs@project': {
                    templateUrl:'resources/html/03_project/tabs/projectTabs_Status.html',
                    controller:'projectTabs_StatusCtrl'
                },
                'projectDetail@project.tabs':{
                    templateUrl:'resources/html/03_project/tabs/proDetail/projectRun.html'
                }
            }
        })
        //工程：运行状况总结
        .state('project.tabs.run',{
            url:'/run',
            views:{
                'projectDetail@project.tabs':{
                    templateUrl:'resources/html/03_project/tabs/proDetail/projectRun.html'
                }
            }
        })
        //工程：工程基本信息
        .state('project.tabs.info',{
            url:'/info',
            views:{
                'projectDetail@project.tabs':{
                    templateUrl:'resources/html/03_project/tabs/proDetail/projectInfo.html',
                    controller:'projectInfoCtrl'
                }
            }
        })
        //工程：报警设置
        .state('project.tabs.warningCinfig',{
            url:'/warningCinfig',
            views:{
                'projectDetail@project.tabs':{
                    templateUrl:'resources/html/03_project/tabs/proDetail/proWarningSet.html'
                }
            }
        })
        //工程：报表
        .state('project.tabs.forms',{
            url:'/forms',
            views:{
                'projectDetail@project.tabs':{
                    templateUrl:'resources/html/03_project/tabs/proDetail/proForms.html'
                }
            }
        })
        //工程：设备健康情况诊断表
        .state('project.tabs.health',{
            url:'/health',
            views:{
                'projectDetail@project.tabs':{
                    templateUrl:'resources/html/03_project/tabs/proDetail/proHealth.html'
                }
            }
        })
        //工程：报警历史日志
        .state('project.tabs.warningLog',{
            url:'/warningLog',
            views:{
                'projectDetail@project.tabs':{
                    templateUrl:'resources/html/03_project/tabs/proDetail/proWarningLog.html'
                }
            }
        })
        //工程下断路器Tab页面
        .state('project.tabs.beakerTabs',{
            url:'/beaker/:breakerId',
            views: {
                'projectTabs@project': {
                    templateUrl: 'resources/html/03_project/tabs/breakerTabs.html',
                    controller:'breakerTabCtrl'
                }/*,
                'breakerDetails@project.tabs.beakerTabs': {
                    templateUrl: 'resources/html/03_project/tabs/machineDetail/breakerRun.html'
                }*/
            }
        })
        //断路器：设备运行情况
        .state('project.tabs.beakerTabs.run',{
            url:'/run',
            views: {
                'breakerDetails@project.tabs.beakerTabs': {
                    templateUrl: 'resources/html/03_project/tabs/machineDetail/breakerRun.html',
                    controller:'breakerRunCtrl'
                }
            }
        })
        //断路器：设备信息
        .state('project.tabs.beakerTabs.info',{
            url:'/info',
            views: {
                'breakerDetails@project.tabs.beakerTabs': {
                    templateUrl: 'resources/html/03_project/tabs/machineDetail/breakerInfo.html',
                    controller:'breakerInfoCtrl'
                }
            }
        })
        //断路器：实时数据
        .state('project.tabs.beakerTabs.realData',{
            url:'/realData',
            views: {
                'breakerDetails@project.tabs.beakerTabs': {
                    templateUrl: 'resources/html/03_project/tabs/machineDetail/breakerRealData.html',
                    controller:'breakerRealDataCtrl'
                }
            }
        })
        //断路器：报警日志
        .state('project.tabs.beakerTabs.warning',{
            url:'/warning',
            views: {
                'breakerDetails@project.tabs.beakerTabs': {
                    templateUrl: 'resources/html/03_project/tabs/machineDetail/breakerWarning.html',
                    controller:'breakerWarningCtrl'
                }
            }
        })
        //断路器：报表
        .state('project.tabs.beakerTabs.forms',{
            url:'/forms',
            views: {
                'breakerDetails@project.tabs.beakerTabs': {
                    templateUrl: 'resources/html/03_project/tabs/machineDetail/breakerForms.html',
                    controller:'breakerFormCtrl'
                }
            }
        })
        //断路器：动态曲线
        .state('project.tabs.beakerTabs.curve',{
            url:'/curve',
            views: {
                'breakerDetails@project.tabs.beakerTabs': {
                    templateUrl: 'resources/html/03_project/tabs/machineDetail/breakerCurve.html'
                    //controller:'breakerInfoCtrl'
                }
            }
        })

        //预测曲线
        .state('predictionCurve',{
            url:'/predictionCurve',
            views:{
                'header':{
                    templateUrl:'html/header.html',
                    controller:'headerCtrl'
                },
                'content':{
                    templateUrl:'html/predictionCurve/predictionCurve.html',
                    controller:'predictionCurveCtrl'
                },
                'footer':{
                    templateUrl:'html/footer.html'
                },
                'leftPredictionCurveMenu@predictionCurve':{
                    templateUrl:'html/predictionCurve/menu/predictionCurveMenu.html'
                },
                'midContent@predictionCurve':{
                    templateUrl:'html/predictionCurve/content/shortManage.html',
                    controller:'shortCurveCtrl'
                }
            }
        })
        /************ begin:预测曲线每个menu对应的html **************/
        //预测曲线管理 ： 短期
        .state('predictionCurve.shortCurve',{
            url:'/shortCurve',
            views:{
                'midContent@predictionCurve':{
                    templateUrl:'html/predictionCurve/content/shortManage.html',
                    controller:'shortCurveCtrl'
                }
            }
        })

        //预测曲线管理 ： 超短期
        .state('predictionCurve.ultraShortCurve',{
            url:'/ultraShortCurve',
            views:{
                'midContent@predictionCurve':{
                    templateUrl:'/html/predictionCurve/content/ultraShortManage.html',
                    controller:'ultraShortCurveCtrl'
                }
            }
        })
        /************ end:预测曲线每个menu对应的html **************/

        //实时曲线
        .state('realtimeCurve',{
            url:'/realtimeCurve',
            views:{
                'header':{
                    templateUrl:'html/header.html',
                    controller:'headerCtrl'
                },
                'content':{
                    templateUrl:'html/realtimeCurve/realtimeCurve.html',
                    controller:'realtimeCurveCtrl'
                },
                'footer':{
                    templateUrl:'html/footer.html'
                },
                'leftRealtimeCurveMenu@realtimeCurve':{
                    templateUrl:'html/realtimeCurve/menu/realtimeCurveMenu.html'
                }
            }
        })

        //实时曲线内容
        .state('realtimeCurve.realtimeCurveContent',{
            url:'/realtimeCurveContent',
            views:{
                'midContent@realtimeCurve':{
                    templateUrl:'html/realtimeCurve/content/realtimeCurveContent.html',
                    controller:'realtimeCurveModalCtrl'
                }
            }
        })

        //报表
        .state('report',{
            url:'/report',
            views:{
                'header':{
                    templateUrl:'html/header.html',
                    controller:'headerCtrl'
                },
                'content':{
                    templateUrl:'html/report/report.html',
                    controller:'reportCtrl'
                },
                'footer':{
                    templateUrl:'html/footer.html'
                },
                'leftReportMenu@report':{
                    templateUrl:'html/report/menu/reportMenu.html'
                }
            }
        })

        /************ begin:报表的每个menu对应的html **************/
        // 日报表
        .state('report.reportOfDay',{
            url:'/reportOfDay',
            views:{
                'midContent@report':{
                    templateUrl:'html/report/content/reportOfDay.html',
                    controller:'reportOfDayModalCtrl'
                }
            }
        })

        // 月报表
        .state('report.reportOfMonth',{
            url:'/reportOfMonth',
            views:{
                'midContent@report':{
                    templateUrl:'html/report/content/reportOfMonth.html',
                    controller:'reportOfMonthModalCtrl'
                }
            }
        })

        // 年报表
        .state('report.reportOfYear',{
            url:'/reportOfYear',
            views:{
                'midContent@report':{
                    templateUrl:'html/report/content/reportOfYear.html',
                    controller:'reportOfYearModalCtrl'
                }
            }
        })
        /************ end:报表的每个menu对应的html **************/


        // 历史查询
        .state('historySearch',{
            url:'/historySearch',
            views:{
                'header':{
                    templateUrl:'html/header.html',
                    controller:'headerCtrl'
                },
                'content':{
                    templateUrl:'html/historySearch/historySearch.html',
                    controller:'historySearchCtrl'
                },
                'footer':{
                    templateUrl:'html/footer.html'
                },
                'leftHistorySearchMenu@historySearch':{
                    templateUrl:'html/historySearch/menu/historySearchMenu.html'
                }
            }
        })

        /************ begin:历史查询的每个menu对应的html **************/
        // 六遥事件
        .state('historySearch.sixEvent',{
            url:'/sixEvent',
            views:{
                'midContent@historySearch':{
                    templateUrl:'html/historySearch/content/sixEvent.html',
                    controller:'sixEventModalCtrl'
                }
            }
        })

        // 用户事件
        .state('historySearch.userEvent',{
            url:'/userEvent',
            views:{
                'midContent@historySearch':{
                    templateUrl:'html/historySearch/content/userEvent.html',
                    controller:'userEventModalCtrl'
                }
            }
        })

        // 文件上送事件
        .state('historySearch.fileDelivery',{
            url:'/fileDelivery',
            views:{
                'midContent@historySearch':{
                    templateUrl:'html/historySearch/content/fileDelivery.html',
                    controller:'fileDeliveryModalCtrl'
                }
            }
        })

        // 气象站数据
        .state('historySearch.weatherStation',{
            url:'/weatherStation',
            views:{
                'midContent@historySearch':{
                    templateUrl:'html/historySearch/content/weatherStation.html',
                    controller:'weatherStationModalCtrl'
                }
            }
        })

        // 天气预报数据
        .state('historySearch.weatherForecast',{
            url:'/weatherForecast',
            views:{
                'midContent@historySearch':{
                    templateUrl:'html/historySearch/content/weatherForecast.html',
                    controller:'weatherForecastModalCtrl'
                }
            }
        })

        // 站内数据
        .state('historySearch.stationData',{
            url:'/stationData',
            views:{
                'midContent@historySearch':{
                    templateUrl:'html/historySearch/content/stationData.html',
                    controller:'stationDataModalCtrl'
                }
            }
        })
        /************ end:历史查询的每个menu对应的html **************/

        // 场站信息
        .state('stationInfo',{
            url:'/stationInfo',
            views:{
                'header':{
                    templateUrl:'html/header.html',
                    controller:'headerCtrl'
                },
                'content':{
                    templateUrl:'html/station/stationInfo.html',
                    controller:'stationInfoCtrl'
                },
                'footer':{
                    templateUrl:'html/footer.html'
                },
                'leftStationInfoMenu@stationInfo':{
                    templateUrl:'html/station/menu/stationInfoMenu.html'
                }
            }
        })

        /************ begin:场站信息的每个menu对应的html **************/
        // 逆变器
        .state('stationInfo.inverterContent',{
            url:'/inverterContent',
            views:{
                'midContent@stationInfo':{
                    templateUrl:'html/station/content/inverterContent.html',
                    controller:'inverterModalCtrl'
                }
            }
        })
        /************ begin:场站信息的每个menu对应的html **************/

        //用户信息管理
        .state('userManage',{
            url:'/userManage',
            views:{
                'header':{
                    templateUrl:'html/header.html',
                    controller:'headerCtrl'
                },
                'content':{
                    templateUrl:'html/userManage/userManage.html',
                    controller:'userMangeCtrl'
                },
                'footer':{
                    templateUrl:'html/footer.html'
                },
                'leftMenu@userManage':{
                    templateUrl:'html/userManage/menu/userInfoMenu.html'
                }
            }
        })
        //用户信息管理 ： 用户管理
        .state('userManage.userInfoManage',{
            url:'/userInfoManage',
            views:{
                'midContent@userManage':{
                    templateUrl:'html/userManage/tabs/admin/userInfoManage.html',
                    controller:'userInfoMangeCtrl'
                }
            }
        })
        //用户信息管理 ： 密码修改
        .state('userManage.padChange',{
            url:'/padChange',
            views:{
                'midContent@userManage':{
                    templateUrl:'html/userManage/tabs/user/passwordChange.html',
                    controller:'userPwdCtrl'
                }
            }
        })


});

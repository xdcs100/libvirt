/*
 * vircgroup.h: methods for managing control cgroups
 *
 * Copyright (C) 2011-2013 Red Hat, Inc.
 * Copyright IBM Corp. 2008
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *  Dan Smith <danms@us.ibm.com>
 */

#ifndef __VIR_CGROUP_H__
# define __VIR_CGROUP_H__

# include "virutil.h"

struct virCgroup;
typedef struct virCgroup *virCgroupPtr;

enum {
    VIR_CGROUP_CONTROLLER_CPU,
    VIR_CGROUP_CONTROLLER_CPUACCT,
    VIR_CGROUP_CONTROLLER_CPUSET,
    VIR_CGROUP_CONTROLLER_MEMORY,
    VIR_CGROUP_CONTROLLER_DEVICES,
    VIR_CGROUP_CONTROLLER_FREEZER,
    VIR_CGROUP_CONTROLLER_BLKIO,
    VIR_CGROUP_CONTROLLER_NET_CLS,
    VIR_CGROUP_CONTROLLER_PERF_EVENT,

    VIR_CGROUP_CONTROLLER_LAST
};

VIR_ENUM_DECL(virCgroupController);

bool virCgroupAvailable(void);

int virCgroupNewPartition(const char *path,
                          bool create,
                          int controllers,
                          virCgroupPtr *group)
    ATTRIBUTE_NONNULL(1) ATTRIBUTE_NONNULL(4);

int virCgroupNewDriver(const char *name,
                       bool create,
                       int controllers,
                       virCgroupPtr *group)
    ATTRIBUTE_NONNULL(1) ATTRIBUTE_NONNULL(4);

int virCgroupNewSelf(virCgroupPtr *group)
    ATTRIBUTE_NONNULL(1);

int virCgroupNewDomainDriver(virCgroupPtr driver,
                             const char *name,
                             bool create,
                             virCgroupPtr *group)
    ATTRIBUTE_NONNULL(1) ATTRIBUTE_NONNULL(2) ATTRIBUTE_NONNULL(4);
int virCgroupNewDomainPartition(virCgroupPtr partition,
                                const char *driver,
                                const char *name,
                                bool create,
                                virCgroupPtr *group)
    ATTRIBUTE_NONNULL(1) ATTRIBUTE_NONNULL(2) ATTRIBUTE_NONNULL(5);

int virCgroupNewVcpu(virCgroupPtr domain,
                     int vcpuid,
                     bool create,
                     virCgroupPtr *group)
    ATTRIBUTE_NONNULL(1) ATTRIBUTE_NONNULL(4);

int virCgroupNewEmulator(virCgroupPtr domain,
                         bool create,
                         virCgroupPtr *group)
    ATTRIBUTE_NONNULL(1) ATTRIBUTE_NONNULL(3);

int virCgroupNewDetect(pid_t pid,
                       virCgroupPtr *group);

bool virCgroupNewIgnoreError(void);

int virCgroupPathOfController(virCgroupPtr group,
                              int controller,
                              const char *key,
                              char **path);

int virCgroupAddTask(virCgroupPtr group, pid_t pid);

int virCgroupAddTaskController(virCgroupPtr group,
                               pid_t pid,
                               int controller);

int virCgroupMoveTask(virCgroupPtr src_group,
                      virCgroupPtr dest_group);

int virCgroupSetBlkioWeight(virCgroupPtr group, unsigned int weight);
int virCgroupGetBlkioWeight(virCgroupPtr group, unsigned int *weight);

int virCgroupSetBlkioDeviceWeight(virCgroupPtr group,
                                  const char *path,
                                  unsigned int weight);

int virCgroupSetMemory(virCgroupPtr group, unsigned long long kb);
int virCgroupGetMemoryUsage(virCgroupPtr group, unsigned long *kb);

int virCgroupSetMemoryHardLimit(virCgroupPtr group, unsigned long long kb);
int virCgroupGetMemoryHardLimit(virCgroupPtr group, unsigned long long *kb);
int virCgroupSetMemorySoftLimit(virCgroupPtr group, unsigned long long kb);
int virCgroupGetMemorySoftLimit(virCgroupPtr group, unsigned long long *kb);
int virCgroupSetMemSwapHardLimit(virCgroupPtr group, unsigned long long kb);
int virCgroupGetMemSwapHardLimit(virCgroupPtr group, unsigned long long *kb);
int virCgroupGetMemSwapUsage(virCgroupPtr group, unsigned long long *kb);

enum {
    VIR_CGROUP_DEVICE_READ  = 1,
    VIR_CGROUP_DEVICE_WRITE = 2,
    VIR_CGROUP_DEVICE_MKNOD = 4,
    VIR_CGROUP_DEVICE_RW    = VIR_CGROUP_DEVICE_READ | VIR_CGROUP_DEVICE_WRITE,
    VIR_CGROUP_DEVICE_RWM   = VIR_CGROUP_DEVICE_RW | VIR_CGROUP_DEVICE_MKNOD,
};

int virCgroupDenyAllDevices(virCgroupPtr group);

int virCgroupAllowDevice(virCgroupPtr group,
                         char type,
                         int major,
                         int minor,
                         int perms);
int virCgroupAllowDeviceMajor(virCgroupPtr group,
                              char type,
                              int major,
                              int perms);
int virCgroupAllowDevicePath(virCgroupPtr group,
                             const char *path,
                             int perms);

int virCgroupDenyDevice(virCgroupPtr group,
                        char type,
                        int major,
                        int minor,
                        int perms);
int virCgroupDenyDeviceMajor(virCgroupPtr group,
                             char type,
                             int major,
                             int perms);
int virCgroupDenyDevicePath(virCgroupPtr group,
                            const char *path,
                            int perms);

int virCgroupSetCpuShares(virCgroupPtr group, unsigned long long shares);
int virCgroupGetCpuShares(virCgroupPtr group, unsigned long long *shares);

int virCgroupSetCpuCfsPeriod(virCgroupPtr group, unsigned long long cfs_period);
int virCgroupGetCpuCfsPeriod(virCgroupPtr group, unsigned long long *cfs_period);

int virCgroupSetCpuCfsQuota(virCgroupPtr group, long long cfs_quota);
int virCgroupGetCpuCfsQuota(virCgroupPtr group, long long *cfs_quota);

int virCgroupGetCpuacctUsage(virCgroupPtr group, unsigned long long *usage);
int virCgroupGetCpuacctPercpuUsage(virCgroupPtr group, char **usage);
int virCgroupGetCpuacctStat(virCgroupPtr group, unsigned long long *user,
                            unsigned long long *sys);

int virCgroupSetFreezerState(virCgroupPtr group, const char *state);
int virCgroupGetFreezerState(virCgroupPtr group, char **state);

int virCgroupSetCpusetMems(virCgroupPtr group, const char *mems);
int virCgroupGetCpusetMems(virCgroupPtr group, char **mems);

int virCgroupSetCpusetCpus(virCgroupPtr group, const char *cpus);
int virCgroupGetCpusetCpus(virCgroupPtr group, char **cpus);

int virCgroupRemoveRecursively(char *grppath);
int virCgroupRemove(virCgroupPtr group);

void virCgroupFree(virCgroupPtr *group);
bool virCgroupHasController(virCgroupPtr cgroup, int controller);

int virCgroupKill(virCgroupPtr group, int signum);
int virCgroupKillRecursive(virCgroupPtr group, int signum);
int virCgroupKillPainfully(virCgroupPtr group);

int virCgroupIsolateMount(virCgroupPtr group,
                          const char *oldroot,
                          const char *mountopts);

#endif /* __VIR_CGROUP_H__ */

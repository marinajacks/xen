#ifndef __ASM_DOMAIN_H__
#define __ASM_DOMAIN_H__

#include <linux/thread_info.h>
#ifdef CONFIG_VTI
#include <asm/vmx_vpd.h>
#include <asm/vmmu.h>
#include <asm/regionreg.h>
#endif // CONFIG_VTI
#include <xen/list.h>

extern void arch_do_createdomain(struct vcpu *);

extern void domain_relinquish_resources(struct domain *);

#ifdef CONFIG_VTI
struct trap_bounce {
	// TO add, FIXME Eddie
};

#define	 PMT_SIZE	(32L*1024*1024)		// 32M for PMT
#endif // CONFIG_VTI

struct arch_domain {
    struct mm_struct *active_mm;
    struct mm_struct *mm;
    int metaphysical_rr0;
    int starting_rid;		/* first RID assigned to domain */
    int ending_rid;		/* one beyond highest RID assigned to domain */
    int rid_bits;		/* number of virtual rid bits (default: 18) */
    int breakimm;
#ifdef  CONFIG_VTI
    int imp_va_msb;
    ia64_rr emul_phy_rr0;
    ia64_rr emul_phy_rr4;
    u64 *pmt;	/* physical to machine table */
#endif  //CONFIG_VTI
    u64 xen_vastart;
    u64 xen_vaend;
    u64 shared_info_va;
#ifdef DOMU_AUTO_RESTART
    u64 image_start;
    u64 image_len;
    u64 entry;
#endif
};
#define starting_rid arch.starting_rid
#define ending_rid arch.ending_rid
#define rid_bits arch.rid_bits
#define xen_vastart arch.xen_vastart
#define xen_vaend arch.xen_vaend
#define shared_info_va arch.shared_info_va

struct arch_vcpu {
#if 1
	TR_ENTRY itrs[NITRS];
	TR_ENTRY dtrs[NDTRS];
	TR_ENTRY itlb;
	TR_ENTRY dtlb;
	unsigned long itlb_pte;
	unsigned long dtlb_pte;
	unsigned long irr[4];
	unsigned long insvc[4];
	unsigned long iva;
	unsigned long dcr;
	unsigned long itc;
	unsigned long domain_itm;
	unsigned long domain_itm_last;
	unsigned long xen_itm;
	unsigned long xen_timer_interval;
#endif
    void *regs;	/* temporary until find a better way to do privops */
    int metaphysical_rr0;		// from arch_domain (so is pinned)
    int metaphysical_saved_rr0;		// from arch_domain (so is pinned)
    int breakimm;			// from arch_domain (so is pinned)
    struct mm_struct *active_mm;
    struct thread_struct _thread;	// this must be last
#ifdef CONFIG_VTI
    void (*schedule_tail) (struct vcpu *);
    struct trap_bounce trap_bounce;
    thash_cb_t *vtlb;
    //for phycial  emulation
    unsigned long old_rsc;
    int mode_flags;

    struct arch_vmx_struct arch_vmx; /* Virtual Machine Extensions */
#endif	// CONFIG_VTI
};

#define active_mm arch.active_mm
//#define thread arch._thread

// FOLLOWING FROM linux-2.6.7/include/sched.h

struct mm_struct {
	struct vm_area_struct * mmap;		/* list of VMAs */
#ifndef XEN
	struct rb_root mm_rb;
#endif
	struct vm_area_struct * mmap_cache;	/* last find_vma result */
	unsigned long free_area_cache;		/* first hole */
	pgd_t * pgd;
	atomic_t mm_users;			/* How many users with user space? */
	atomic_t mm_count;			/* How many references to "struct mm_struct" (users count as 1) */
	int map_count;				/* number of VMAs */
#ifndef XEN
	struct rw_semaphore mmap_sem;
#endif
	spinlock_t page_table_lock;		/* Protects task page tables and mm->rss */

	struct list_head mmlist;		/* List of all active mm's.  These are globally strung
						 * together off init_mm.mmlist, and are protected
						 * by mmlist_lock
						 */

	unsigned long start_code, end_code, start_data, end_data;
	unsigned long start_brk, brk, start_stack;
	unsigned long arg_start, arg_end, env_start, env_end;
	unsigned long rss, total_vm, locked_vm;
	unsigned long def_flags;

	unsigned long saved_auxv[40]; /* for /proc/PID/auxv */

	unsigned dumpable:1;
#ifdef CONFIG_HUGETLB_PAGE
	int used_hugetlb;
#endif
#ifndef XEN
	cpumask_t cpu_vm_mask;

	/* Architecture-specific MM context */
	mm_context_t context;

	/* coredumping support */
	int core_waiters;
	struct completion *core_startup_done, core_done;

	/* aio bits */
	rwlock_t		ioctx_list_lock;
	struct kioctx		*ioctx_list;

	struct kioctx		default_kioctx;
#endif
};

extern struct mm_struct init_mm;

#include <asm/uaccess.h> /* for KERNEL_DS */
#include <asm/pgtable.h>

#endif /* __ASM_DOMAIN_H__ */

/*
 * Local variables:
 * mode: C
 * c-set-style: "BSD"
 * c-basic-offset: 4
 * tab-width: 4
 * indent-tabs-mode: nil
 * End:
 */
